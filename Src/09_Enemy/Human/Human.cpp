#include "Human.h"

#include "FunShape.h"
#include "../../08_Player/PHP.h"
#include "../../11_GameSystem/VisionSystem.h"
#include "State/HumanState.h"
#include "../System/AnimalManager.h"
#include "../../06_GameLib/BBox.h"

namespace
{
    constexpr float LINE_LENGTH = 7.0f;
}

CHuman::CHuman(VECTOR3 pos, VECTOR2 areaSize)
    : m_AreaSize(areaSize)
{
    transform.position = pos;
    m_pMesh = ObjectManager::FindGameObject<CAnimalManager>()->MeshList("Human");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    
    m_dwColor = 100;
    angle = 0.0f;

    m_cubeStates[CBaseState::Type::IDLE] = new CHumanIdleState(this);
    m_cubeStates[CBaseState::Type::WALK] = new CHumanWalkState(this);
    m_cubeStates[CBaseState::Type::FIND_PLAYER] = new CHumanFindPlayer(this);
    m_pCurrentState = m_cubeStates[CBaseState::Type::IDLE];
    m_pCurrentState->Enter();
    m_pCurrentState->SetNextState();
    m_pFunShape = new CFunShape();
    m_pBBox = CreateBBox();
}

CHuman::~CHuman()
{
    for (auto& state : m_cubeStates)
    {
        if (state.second == nullptr) continue;
        SAFE_DELETE(state.second);
        state.second = nullptr;
    }
    m_cubeStates.clear();
}

void CHuman::CollisionCheck()
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

void CHuman::Update()
{
    m_pAnimator->Update();
    CVisionSystem*vision = ObjectManager::FindGameObject<CVisionSystem>();
    m_inSight = vision->SectorCircleCollision(ToVec2XZ(transform.position), transform.rotation.y)
        && ObjectManager::FindGameObject<CPlayer>()->GetIsSuckUp();


    if (m_inSight)
    {
        m_dwColor = 255;
        SetState(CBaseState::Type::FIND_PLAYER);
    }
    else
    {
        m_dwColor = 0;
        ObjectManager::FindGameObject<CPlayerHP>()->ResetFlag();
    }
    AtkArea();
    CollisionCheck();
    UpdateBBox();
}


void CHuman::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
    DrawDirectionLine();
    //FanShape();
}

void CHuman::AtkArea() const
{
    m_pFunShape->PosSet(transform.position, angle + transform.rotation.y);
}

//Humanの範囲をLineで可視化
//範囲内なら水色、外なら緑になる
void CHuman::DrawDirectionLine()
{
    CSprite spr;
    MATRIX4X4 mat = XMMatrixRotationY(angle + transform.rotation.y);

    VECTOR3 startPos = transform.position;

    VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;

    spr.DrawLine3D(startPos, endPos, RGB(0, 255, m_dwColor));
}


// void CHuman::FanShape()
// {
//     CSprite spr;
//     
//     float angle = -(20.0f * DegToRad);
//     
//     for (int i = 0; i < 3; i++)
//     {
//         if (i == 2) angle = std::abs(angle);
//
//         MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y + angle);
//         
//         VECTOR3 startPos = transform.position;
//         VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;
//
//         spr.DrawLine3D(startPos, endPos, RGB(255, 0, 0));
//     }
// }
