#include "Player.h"
#include "PCamera.h"
#include <iostream>


#include "../06_GameLib/Lerp.h"
#include "../11_GameSystem/VisionSystem.h"

namespace
{
    constexpr float LINE_LENGTH = 7;
    constexpr float HUMAN_ANGLE = 20;
    const VECTOR3 RAY_LNEGTH = VECTOR3(0, 0, 7);
}

CPlayer::CPlayer()
{
    transform.position = VECTOR3(0, 5, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Mousey/Mousey.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(0, "data/Mousey/Anim_Run.anmx", true);
    m_pAnimator->Play(0);
    m_coneDegree = 20;
    m_allExp = 1;
    m_exp = 0;
    m_hp = 1;
}

CPlayer::~CPlayer() = default;

void CPlayer::Update()
{
    m_pAnimator->Update();
    if (not m_SuctionActive)
    {
        HandleMovementInput();
    }
    m_SuctionActive = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_J);
    // コーンの半径を計算
    m_coneRadius = transform.position.y * tan(DegToRad * m_coneDegree);

    CheckLevel();

    // カメラ位置を更新
    UpdateCameraPos();
    if (m_hp <= 0)
    {
        SceneManager::ChangeScene("TitleScene");
    }

    ObjectManager::FindGameObject<CVisionSystem>()->
            SetCircleCenter(transform.position);
    ObjectManager::FindGameObject<CVisionSystem>()->
        SetCircleRadius(m_coneRadius);
}

void CPlayer::Draw()
{
    Object3D::Draw();
    DrawCircle(VECTOR3(transform.position.x, 0, transform.position.z), m_coneRadius, RGB(0, 255, 0));
}

// void CPlayer::Draw()
// {
//    
//     
// }

void CPlayer::HandleMovementInput()
{
    const float moveSpeed = 5.0f; // 秒間5ユニットの移動速度
    const float moveAmount = moveSpeed * SceneManager::DeltaTime();
    auto* input = GameDevice()->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) transform.position.z += moveAmount;
    if (input->CheckKey(KD_DAT, DIK_S)) transform.position.z -= moveAmount;
    if (input->CheckKey(KD_DAT, DIK_A)) transform.position.x -= moveAmount;
    if (input->CheckKey(KD_DAT, DIK_D)) transform.position.x += moveAmount;
}

void CPlayer::CheckLevel()
{
    if (m_exp >= m_allExp)
    {
        int tmp = m_exp - m_allExp;
        m_allExp *= 1.3;
        m_exp = tmp;
        IncreaseSuctionConeHeight();
    }
}

void CPlayer::IncreaseSuctionConeHeight()
{
    transform.position.y += 3.0f;

    // 半径を更新//
    m_coneRadius = transform.position.y * tan(DegToRad * m_coneDegree);
}

void CPlayer::UpdateCameraPos()
{
    // カメラ位置をコーンの高さに基づいて設定//
    ObjectManager::FindGameObject<CPlayerCamera>()->PosSet(
        transform.position, transform.position.y);
}


//吸い込むスピードを計算
//
VECTOR3 CPlayer::CalcSuctionDisplacement(const float& moveTimeSecond, const VECTOR3& animalPos) const
{
    //Y座標が0の点（求めるための内分比の係数//
    float projectionFactorY0 = avoidZero((0 - animalPos.y) / (animalPos.y - transform.position.y));
    //0地点での動物のポジション
    VECTOR3 targetPointOnPlane =
        VECTOR3(animalPos.x + projectionFactorY0 * (animalPos.x - transform.position.x), 0,
                animalPos.z + projectionFactorY0 * (animalPos.z - transform.position.z));
    VECTOR3 pullVectorToTarget = transform.position - targetPointOnPlane;
    VECTOR3 suctionDisplacementPerFrame = pullVectorToTarget / moveTimeSecond;
    return suctionDisplacementPerFrame * SceneManager::DeltaTime();
}

//エリア内にいるかチェック
//
bool CPlayer::IsWithSuctionCone(const VECTOR3& targetPos) const
{
    const float distanceAnimalFromPlayer = transform.position.y - targetPos.y;
    const float coneRadiusAtTargetHeight = distanceAnimalFromPlayer * std::tan(DegToRad * m_coneDegree);
    if (Pow2(targetPos.x - transform.position.x) + Pow2(targetPos.z - transform.position.z)
        <= Pow2(coneRadiusAtTargetHeight))
    {
        return true;
    }
    return false;
}

bool CPlayer::UFOInVisionFan(const float& humanRotateY, const VECTOR3& targetPosition)
{
    const float distanceHumanFromPlayer = CalcDistanceXZ(transform.position, targetPosition);
    VECTOR3 rayEndPosition = targetPosition + RAY_LNEGTH * XMMatrixRotationY(humanRotateY);
    const VECTOR2 vectorFromTargetToRayEnd = VECTOR2(rayEndPosition.x - targetPosition.x,
                                                     rayEndPosition.z - targetPosition.z);
    const VECTOR2 vectorFromTargetToPlayer = VECTOR2(transform.position.x - targetPosition.x,
                                                     transform.position.z - targetPosition.z);

    if (IsBeyondMaxDistance(distanceHumanFromPlayer) || IsBeyondInsideFanShapeAngle(
        vectorFromTargetToRayEnd, vectorFromTargetToPlayer))
    {
        return false;
    }
    return true;
}

bool CPlayer::IsBeyondMaxDistance(const float& dis)
{
    if (LINE_LENGTH < dis)
    {
        return true;
    }
    return false;
}

bool CPlayer::IsBeyondInsideFanShapeAngle(const VECTOR2& vectorTargetToRayEnd, const VECTOR2& vectorTargetToPlayer)
{
    const VECTOR2 HumanFromPlayerNorm = normalize(vectorTargetToRayEnd);
    const VECTOR2 HumanFromEndPosNorm = normalize(vectorTargetToPlayer);
    const float anglePlayerFromEndPos = CalcVector2Angle(HumanFromPlayerNorm, HumanFromEndPosNorm);
    if (HUMAN_ANGLE < anglePlayerFromEndPos)
    {
        return true;
    }
    return false;
}





void CPlayer::DrawCircle(const VECTOR3& center, float radius, DWORD color)
{
    CSprite spr;
    constexpr int segments = 32; // 円を構成する線分の数
    const float angleStep = 2.0f * 3.14159f / segments; // 各線分の角度

    for (int i = 0; i < segments; ++i)
    {
        // 現在の点
        float angle1 = i * angleStep;
        VECTOR3 point1 = center;
        point1.x += radius * cos(angle1);
        point1.z += radius * sin(angle1);

        // 次の点
        float angle2 = (i + 1) % segments * angleStep;
        VECTOR3 point2 = center;
        point2.x += radius * cos(angle2);
        point2.z += radius * sin(angle2);

        // 線を描画
        spr.DrawLine3D(point1, point2, color);
    }
    spr.DrawLine3D(transform.position, VECTOR3(center.x + m_coneRadius, center.y, center.z), color);
    spr.DrawLine3D(transform.position, VECTOR3(center.x - m_coneRadius, center.y, center.z), color);
    spr.DrawLine3D(transform.position, VECTOR3(center.x, center.y, center.z + m_coneRadius), color);
    spr.DrawLine3D(transform.position, VECTOR3(center.x, center.y, center.z - m_coneRadius), color);
}
