#include "TutorialAnimal.h"

#include <algorithm>
#include <cfloat>

#include "../System/GameInstance.h"
#include "../Player/Player.h"
#include "../Utils/Animator.h"
#include "../Enemies/System/EnemyManager.h"
#include "../Stage/GroundHitResult.h"


CTutorialAnimal::CTutorialAnimal(const VECTOR3& pos)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);

    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);

    transform.position = pos;
    transform.rotation = VECTOR3(0, XM_PI, 0);

    m_pEnemyManager = ObjectManager::FindGameObject<CEnemyManager>();
}

void CTutorialAnimal::Update()
{
    CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();

    // 吸い込み範囲内かつ、吸い込みボタンを押していたら位置を加算していく //
    if (pPl->IsWithSuctionCone(transform.position) && pPl->GetIsSuckUp())
    {
        // 吸い込み時の移動速度係数 //
        constexpr int SUCTION_SPEED_FACTOR = 1;

        transform.position += pPl->CalcSuctionDisplacement(SUCTION_SPEED_FACTOR, transform.position);
        m_velocityY = 0.0f; // 吸い込み中は重力をリセット //
    }
    else
    {
        ApplyGravity();
    }
}

////////////////////
// Scoreを加算してオブジェクトをDestroy
////////////////////
void CTutorialAnimal::Destroy()
{
    // 捕獲時に加算する経験値 //
    constexpr int CAPTURE_EXP = 1;
    // 捕獲時に加算するスコア //
    constexpr int CAPTURE_SCORE = 100;
    // 捕獲時に加算する捕獲数 //
    constexpr int CAPTURE_COUNT = 1;

    ObjectManager::FindGameObject<CPlayerLevel>()->AddExp(CAPTURE_EXP);
    CGameInstance::Get()->AddScore(CAPTURE_SCORE);
    CGameInstance::Get()->AddCapture(CAPTURE_COUNT);
    DestroyMe();
}

CTutorialAnimal::~CTutorialAnimal() = default;

////////////////////
// 重力を適用する
////////////////////
void CTutorialAnimal::ApplyGravity()
{
    static constexpr float GRAVITY = 9.8f;
    static constexpr float GROUND_CHECK_OFFSET = 0.1f;
    static constexpr float GROUND_SKIN = 0.02f;

    m_velocityY -= GRAVITY * SceneManager::DeltaTime();
    const float dt = SceneManager::DeltaTime();
    const float nextY = transform.position.y + m_velocityY * dt;

    if (m_pEnemyManager != nullptr)
    {
        const float fromY = transform.position.y + GROUND_CHECK_OFFSET;
        const float toY = nextY - GROUND_CHECK_OFFSET;

        if (toY < fromY)
        {
            VECTOR2 pos, size;
            GroundHitResult hit;
            if (GetBounds2D(pos, size) &&
                m_pEnemyManager->FindGroundBelow(pos, size, transform.position, fromY, toY, &hit))
            {
                transform.position.y = hit.y + GROUND_SKIN;
                m_velocityY = 0.0f;
                return;
            }
        }
    }

    transform.position.y = nextY;
}

bool CTutorialAnimal::GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const
{
    if (m_pMesh == nullptr)
    {
        return false;
    }

    const VECTOR3 min = m_pMesh->m_vMin;
    const VECTOR3 max = m_pMesh->m_vMax;

    VECTOR3 corners[4] = {
        VECTOR3(min.x, 0, min.z),
        VECTOR3(max.x, 0, min.z),
        VECTOR3(min.x, 0, max.z),
        VECTOR3(max.x, 0, max.z)
    };

    MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y);
    MATRIX4X4 scaleM = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
    MATRIX4X4 transformMatrix = scaleM * rotY;

    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minZ = FLT_MAX, maxZ = -FLT_MAX;

    for (const auto& corner : corners)
    {
        VECTOR3 transformed = XMVector3TransformCoord(corner, transformMatrix);
        minX = (std::min)(minX, transformed.x);
        maxX = (std::max)(maxX, transformed.x);
        minZ = (std::min)(minZ, transformed.z);
        maxZ = (std::max)(maxZ, transformed.z);
    }

    outPos = VECTOR2(
        (minX + maxX) * 0.5f + transform.position.x,
        (minZ + maxZ) * 0.5f + transform.position.z
    );
    outSize = VECTOR2(maxX - minX, maxZ - minZ);
    return true;
}
