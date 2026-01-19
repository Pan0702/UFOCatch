#include "TutorialAnimal.h"

#include "../11_GameSystem/GameInstance.h""
#include "../08_Player/Player.h"
#include "../06_GameLib/Animator.h"
#include "../09_Enemy/System/AnimalManager.h"
#include "../10_Stage/Ground.h"

namespace
{
    // 吸い込み時の移動速度係数 //
    constexpr int SUCTION_SPEED_FACTOR = 1;
    // 捕獲時に加算する経験値 //
    constexpr int CAPTURE_EXP = 1;
    // 捕獲時に加算するスコア //
    constexpr int CAPTURE_SCORE = 100;
    // 捕獲時に加算する捕獲数 //
    constexpr int CAPTURE_COUNT = 1;
}

CTutorialAnimal::CTutorialAnimal(const VECTOR3& pos)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);

    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);

    transform.position = pos;
    transform.rotation = VECTOR3(0, XM_PI, 0);

    m_pGround = ObjectManager::FindGameObject<CGround>();
}

void CTutorialAnimal::Update()
{
    CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
    // 吸い込む範囲内かつ吸い込むボタンを押していたら位置を足していく //
    if (pPl->IsWithSuctionCone(transform.position) && pPl->GetIsSuckUp())
    {
        transform.position += pPl->CalcSuctionDisplacement(SUCTION_SPEED_FACTOR, transform.position);
        m_velocityY = 0.0f;  // 吸い込み中は重力をリセット //
    }
    else
    {
        ApplyGravity();
    }
}

////////////////////
// Scoreを足してオブジェクトをDestroy //
////////////////////
void CTutorialAnimal::Destroy()
{
    ObjectManager::FindGameObject<CPlayer>()->AddExp(CAPTURE_EXP);
    ObjectManager::FindGameObject<CGameInstance>()->AddScore(CAPTURE_SCORE);
    ObjectManager::FindGameObject<CGameInstance>()->AddCapture(CAPTURE_COUNT);
    DestroyMe();
}

CTutorialAnimal::~CTutorialAnimal() = default;

////////////////////
// 重力を適用する //
////////////////////
void CTutorialAnimal::ApplyGravity()
{
    static constexpr float GRAVITY = 9.8f;
    static constexpr float GROUND_CHECK_OFFSET = 0.1f;
    static constexpr float GROUND_SKIN = 0.02f;

    m_velocityY -= GRAVITY * SceneManager::DeltaTime();
    const float dt = SceneManager::DeltaTime();
    const float nextY = transform.position.y + m_velocityY * dt;

    if (m_pGround != nullptr)
    {
        const float fromY = transform.position.y + GROUND_CHECK_OFFSET;
        const float toY = nextY - GROUND_CHECK_OFFSET;

        if (toY < fromY)
        {
            const VECTOR3 rayStart = VECTOR3(transform.position.x, fromY, transform.position.z);
            const VECTOR3 rayEnd = VECTOR3(transform.position.x, toY, transform.position.z);

            MeshCollider::CollInfo collInfo;
            if (m_pGround->HitLineToMesh(rayStart, rayEnd, &collInfo))
            {
                transform.position.y = collInfo.hitPosition.y + GROUND_SKIN;
                m_velocityY = 0.0f;
                return;
            }
        }
    }

    transform.position.y = nextY;
}
