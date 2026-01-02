#include "ACube.h"

#include "../../08_Player/Player.h"
#include "../../10_Stage/Ground.h"
#include <queue>
#include <thread>

#include "../System/AnimalManager.h"
#include "../Base/StateBase.h"
#include "State/CubeState.h"
#include "../../06_GameLib/BBox.h"

CACube::CACube(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_moveAreaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CAnimalManager>()->MeshList("Dog");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();

    m_cubeStates[CBaseState::Type::IDLE] = new CCubeIdleState(this);
    m_cubeStates[CBaseState::Type::WALK] = new CCubeWalkState(this);
    m_cubeStates[CBaseState::Type::SUCTION] = new CCubeSuction(this);
    m_cubeStates[CBaseState::Type::DESTROY] = new CCubeDestroy(this);
    m_pCurrentState = m_cubeStates[CBaseState::Type::IDLE];
    m_pCurrentState->Enter();
    m_pBBox = CreateBBox();
}

CACube::~CACube()
{
    for (auto& state : m_cubeStates)
    {
        SAFE_DELETE(state.second);
    }
}


void CACube::Update()
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    CEnemyBase::Update();

    m_pAnimator->Update();
    CollisionCheck();
    UpdateBBox();
}

void CACube::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
}

void CACube::IsSuctionCheck()
{
    if (m_pPlayer == nullptr)return;
    if (m_pPlayer->IsWithSuctionCone(transform.position /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CBaseState::Type::SUCTION);
    }
}

VECTOR3 CACube::SuctionSpeed() const
{
    return m_pPlayer->
        CalcSuctionDisplacement(1, transform.position);
}
void CACube::DestroyCube()
{
    DestroyMe();
}

void CACube::CollisionCheck()
{
    // 四分木から周辺のエネミーを効率的に取得
    std::vector<CEnemyBase*> nearbyEnemies = GetNearbyEnemies();
    // 周辺エネミーと当たり判定
    for (auto* enemy : nearbyEnemies)
    {
        if (enemy == this) continue;  // 自分自身はスキップ
        if (enemy->GetBBox() == nullptr) continue;

        VECTOR3 hitPos, hitNormal;
        if (m_pBBox->OBBCollisionDetection(enemy->GetBBox(), &hitPos, &hitNormal))
        {
            // 衝突した！
            // 自分と相手のOBB中心座標を計算
            MATRIX4X4 myCenterMat = XMMatrixTranslation(
                m_pBBox->m_fLengthX + m_pBBox->m_vMin.x,
                m_pBBox->m_fLengthY + m_pBBox->m_vMin.y,
                m_pBBox->m_fLengthZ + m_pBBox->m_vMin.z
            );
            myCenterMat = myCenterMat * m_pBBox->m_mWorld;
            VECTOR3 myCenter = VECTOR3(myCenterMat._41, myCenterMat._42, myCenterMat._43);

            CBBox* enemyBBox = enemy->GetBBox();
            MATRIX4X4 enemyCenterMat = XMMatrixTranslation(
                enemyBBox->m_fLengthX + enemyBBox->m_vMin.x,
                enemyBBox->m_fLengthY + enemyBBox->m_vMin.y,
                enemyBBox->m_fLengthZ + enemyBBox->m_vMin.z
            );
            enemyCenterMat = enemyCenterMat * enemyBBox->m_mWorld;
            VECTOR3 enemyCenter = VECTOR3(enemyCenterMat._41, enemyCenterMat._42, enemyCenterMat._43);

            // 押し戻しベクトルを計算（自分から相手への方向の逆）
            VECTOR3 pushDirection = myCenter - enemyCenter;
            pushDirection.y = 0.0f;  // Y成分を無視してXZ平面のみで押し戻す
            float distance = magnitude(pushDirection);

            if (distance > 0.001f)  // normalize()内でのゼロ除算を避ける
            {
                pushDirection = normalize(pushDirection);

                // 押し戻し距離（フレーム毎に少しずつ押し戻す）
                float pushDistance = 0.1f * SceneManager::DeltaTime() * 60.0f;

                // 位置を更新
                transform.position += pushDirection * pushDistance;
            }
        }
    }
}

bool CACube::ShouldApplyGravity() const
{
    return m_pCurrentState != m_cubeStates.at(CBaseState::Type::SUCTION);
}
