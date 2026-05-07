#include "ConeDraw.h"
#include "../Framework/ObjectManager.h"
#include "../Core/Game/GameMain.h"
#include "../Utils/Sprite3D.h"
#include "../Utils/FbxMesh.h"

CConeDraw::CConeDraw(float coneTopPos, CPlayer* p)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Player/Cone2.mesh");
    m_pMesh->SetLightIntensity(1.0f, 1.0f, 1.0f, 1.0f);
    transform.position = VECTOR3(0, 0, 0);
    m_pPlayer = p;
    m_pLevel = ObjectManager::FindGameObject<CPlayerLevel>();
    transform.scale.y = coneTopPos;
    SetDrawOrder(-1);
}

CConeDraw::~CConeDraw()
{
    SAFE_DELETE(m_pMesh);
}

void CConeDraw::Start()
{
    SetDrawOrder(-10);
}

void CConeDraw::Update()
{
    Object3D::Update();
    if (m_pLevel)
    {
        const float radius = m_pLevel->GetRadius();
        transform.scale = VECTOR3(radius * 2, transform.scale.y, radius * 2);
    }
    if (m_pPlayer)
    {
        transform.position = VECTOR3(m_pPlayer->GetTransform().position.x, 0, m_pPlayer->GetTransform().position.z);
    }
}

void CConeDraw::Draw()
{
    if (m_pPlayer)
    {
        if (m_pPlayer->GetIsSuckUp())
        {
            Object3D::Draw();
        }
    }
}

////////////////////
// 地面に吸引範囲の円を描画する //
////////////////////


CCircleDraw::CCircleDraw(CPlayer* p)
{
    SetDrawOrder(1);
    m_pPlayer = p;
    m_pLevel = ObjectManager::FindGameObject<CPlayerLevel>();
    m_pCircleImage = std::make_unique<CSpriteImage>(TEXT("data/CircleSuction.png"));
}

CCircleDraw::~CCircleDraw()
{
}

void CCircleDraw::Start()
{
    SetDrawOrder(-10);
}

void CCircleDraw::Update()
{
    Object3D::Update();
}

void CCircleDraw::Draw()
{
    if (!m_pPlayer)
    {
        return;
    }
    if (!m_pCircleImage)
    {
        printf("CCircleDraw");
        return;
    }
    CSprite spr;
    VECTOR3 plPos = m_pPlayer->GetPos();
    spr.DrawWorld(m_pCircleImage.get(), VECTOR3(plPos.x, 0, plPos.z), m_pLevel->GetRadius(), 0.7f);
}

///Debug///
// void CConeDraw::DrawDebugCone() const
// {
//     CSprite spr;
//     const float   radius    = m_pLevel->GetRadius();
//     const float   coneTop   = m_pLevel->GetConeTopPos();
//     const VECTOR3 playerPos = m_pPlayer->GetPos();
//     const VECTOR3 center    = VECTOR3(playerPos.x, 0, playerPos.z);
//
//     constexpr int   segments  = 32;
//     const     float angleStep = 2.0f * 3.14159f / segments;
//
//     for (int i = 0; i < segments; ++i)
//     {
//         float   angle1 = static_cast<float>(i) * angleStep;
//         VECTOR3 point1 = center;
//         point1.x += radius * cos(angle1);
//         point1.z += radius * sin(angle1);
//
//         float   angle2 = (i + 1) % segments * angleStep;
//         VECTOR3 point2 = center;
//         point2.x += radius * cos(angle2);
//         point2.z += radius * sin(angle2);
//
//         spr.DrawLine3D(point1, point2, RGB(0, 255, 0));
//     }
//
//     const VECTOR3 topPos = VECTOR3(playerPos.x, coneTop, playerPos.z);
//     spr.DrawLine3D(topPos, VECTOR3(center.x + radius, center.y, center.z), RGB(0, 255, 0));
//     spr.DrawLine3D(topPos, VECTOR3(center.x - radius, center.y, center.z), RGB(0, 255, 0));
//     spr.DrawLine3D(topPos, VECTOR3(center.x, center.y, center.z + radius), RGB(0, 255, 0));
//     spr.DrawLine3D(topPos, VECTOR3(center.x, center.y, center.z - radius), RGB(0, 255, 0));
// }
