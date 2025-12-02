#define NOMINMAX
#include "Player.h"
#include "PCamera.h"
#include <iostream>
#include <algorithm>
#include "PHP.h"
#include "../06_GameLib/Lerp.h"
#include "../11_GameSystem/VisionSystem.h"

CPlayer::CPlayer()
{
    transform.position = VECTOR3(0, 5, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Player/UFO.mesh");
    new CPlayerHP(1000);
    m_coneDegree = 7;
    //半径の計算//
    m_coneRadius = transform.position.y * tan(DegToRad * m_coneDegree);
    m_allExp = 1;
    m_exp = 0.01;
    m_zoomUp = false;
    m_coneTopPos = transform.position.y;
    transform.scale = VECTOR3(0.5f,0.5f,0.5f);
    m_SuctionActive = false;
}

CPlayer::~CPlayer() = default;

void CPlayer::Update()
{
    if (not m_SuctionActive)
    {
        HandleMovementInput();
    }
    m_SuctionActive = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_J);

    CheckLevel();

    // Lerp処理//
    UpdateHeightAndRadiusLerp();

    // カメラ位置を更新//
    UpdateCameraPos();

    ObjectManager::FindGameObject<CVisionSystem>()->
            SetCircleCenter(transform.position);
    ObjectManager::FindGameObject<CVisionSystem>()->
        SetCircleRadius(m_coneRadius);
}

void CPlayer::HandleMovementInput()
{
    // 位置を-50.0f ~ 50.0fの範囲内に制限//
    constexpr float maxPos = 20.0f;
    transform.position.x = std::max(-maxPos,std::min(maxPos,transform.position.x));
    transform.position.z = std::max(-maxPos,std::min(maxPos,transform.position.z));
    constexpr  float moveSpeed = 5.0f; // 秒間5ユニットの移動速度//
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
        float tmp = m_exp - m_allExp;
        m_allExp *= 1.3f;
        m_exp = tmp;
        IncreaseSuctionConeHeight();
    }
}

void CPlayer::IncreaseSuctionConeHeight()
{
    const float targetHeight = m_coneTopPos+ 1.0f;
    const float targetRadius = targetHeight * tan(DegToRad * m_coneDegree);
    
    m_heightLerp.Start(m_coneTopPos, targetHeight, 0.05f);
    m_radiusLerp.Start(m_coneRadius, targetRadius, 0.05f);
}

void CPlayer::UpdateHeightAndRadiusLerp()
{
    if (m_heightLerp.IsLerping())
    {
        m_coneTopPos = m_heightLerp.Update(SceneManager::DeltaTime());
    }
    if (m_radiusLerp.IsLerping())
    {
        m_coneRadius = m_radiusLerp.Update(SceneManager::DeltaTime());
    }
}

void CPlayer::UpdateCameraPos() 
{
    if (not m_SuctionActive)
    {
        if (m_zoomUp)
        {
            ObjectManager::FindGameObject<CPlayerCamera>()->ZoomOut(transform.position);
            m_zoomUp = false;
        }else
        {
            // カメラ位置をコーンの高さに基づいて設定//
            ObjectManager::FindGameObject<CPlayerCamera>()->PosSet(
                transform.position, m_coneTopPos);
        }
    }else
    {
        m_zoomUp = true;
        ObjectManager::FindGameObject<CPlayerCamera>()->ZoomIn(transform.position);
    }
}



//吸い込むスピードを計算
//高さの差が大きいほど遅く、近いほど速く吸い込む//
VECTOR3 CPlayer::CalcSuctionDisplacement(const float& moveTimeSecond, const VECTOR3& animalPos) const
{

    float heightDiff = m_coneTopPos - animalPos.y;
    float progress = 1.0f - (heightDiff / m_coneTopPos);
    progress = std::max(0.0f, std::min(1.0f, progress));
    float eased = progress * progress * progress ;

    // 速度係数を計算//
    constexpr float minSpeed = 0.4f;  
    constexpr float maxSpeed = 1.8f;  
    float heightSpeedMultiplier = minSpeed + (maxSpeed - minSpeed) * eased;

    //Y座標が0の点（求めるための内分比の係数//
    float projectionFactorY0 = avoidZero((0 - animalPos.y) / (animalPos.y - m_coneTopPos));
    //0地点での動物のポジション//
    VECTOR3 targetPointOnPlane =
        VECTOR3(animalPos.x + projectionFactorY0 * (animalPos.x - transform.position.x), 0,
                animalPos.z + projectionFactorY0 * (animalPos.z - transform.position.z));
    VECTOR3 pullVectorToTarget = transform.position - targetPointOnPlane;
    
    VECTOR3 suctionDisplacementPerFrame = pullVectorToTarget / moveTimeSecond * heightSpeedMultiplier;
    return suctionDisplacementPerFrame * SceneManager::DeltaTime();
}

//エリア内にいるかチェック
//
bool CPlayer::IsWithSuctionCone(const VECTOR3& targetPos) const
{
    const float distanceAnimalFromPlayer = m_coneTopPos - targetPos.y;
    const float coneRadiusAtTargetHeight = distanceAnimalFromPlayer * std::tan(DegToRad * m_coneDegree);
    if (Pow2(targetPos.x - transform.position.x) + Pow2(targetPos.z - transform.position.z)
        <= Pow2(coneRadiusAtTargetHeight))
    {
        return true;
    }
    return false;
}

void CPlayer::Draw()
{
    Object3D::Draw();
    DrawCircle(VECTOR3(transform.position.x, 0, transform.position.z), m_coneRadius, RGB(0, 255, 0));
}

///Debug///
void CPlayer::DrawCircle(const VECTOR3& center, float radius, DWORD color)
{
    CSprite spr;
    constexpr int segments = 32; // 円を構成する線分の数
    const float angleStep = 2.0f * 3.14159f / segments; // 各線分の角度

    for (int i = 0; i < segments; ++i)
    {
        // 現在の点
        float angle1 = static_cast<float>(i) * angleStep;
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
