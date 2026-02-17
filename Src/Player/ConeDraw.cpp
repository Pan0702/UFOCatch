#include "ConeDraw.h"

CConeDraw::CConeDraw(float coneTopPos)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Player/Cone2.mesh");
    m_pMesh->SetLightIntensity(1.0f, 1.0f, 1.0f, 1.0f);
    transform.position = VECTOR3(0, 0, 0);
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pLevel  = ObjectManager::FindGameObject<CPlayerLevel>();
    transform.scale.y = coneTopPos;
    SetDrawOrder(-1);

    m_pCircleImage = new CSpriteImage(TEXT("data/CircleSuction.png"));
}

CConeDraw::~CConeDraw()
{
    SAFE_DELETE(m_pMesh);
    SAFE_DELETE(m_pCircleImage);
}

void CConeDraw::Update()
{
    Object3D::Update();
    const float radius = m_pLevel->GetRadius();
    transform.scale = VECTOR3(radius * 2, transform.scale.y, radius * 2);
    transform.position = VECTOR3(m_pPlayer->GetTransform().position.x, 0,
                                 m_pPlayer->GetTransform().position.z);
}

void CConeDraw::Draw()
{
    if (m_pPlayer->GetIsSuckUp()) Object3D::Draw();

    DrawSuctionCircle();
    DrawDebugCone();
}

////////////////////
// 地面に吸い込み範囲の円を描画する //
////////////////////
void CConeDraw::DrawSuctionCircle() const
{
    static constexpr float GROUND_OFFSET        = 0.1f;
    static constexpr float CIRCLE_DIAMETER_SCALE = 2.0f;
    static constexpr float CIRCLE_DEPTH         = 1.0f;
    static constexpr float GROUND_ROTATION      = -XM_PI / 2.0f;
    static constexpr float CIRCLE_ALPHA         = 0.5f;
    static constexpr float SPRITE_SIZE          = 1.0f;

    CSprite spr;

    const float   radius    = m_pLevel->GetRadius();
    const VECTOR3 playerPos = m_pPlayer->GetPos();
    const VECTOR3 groundPos = VECTOR3(playerPos.x, GROUND_OFFSET, playerPos.z);

    const MATRIX4X4 mScale       = XMMatrixScaling(radius * CIRCLE_DIAMETER_SCALE,
                                                    radius * CIRCLE_DIAMETER_SCALE,
                                                    CIRCLE_DEPTH);
    const MATRIX4X4 mRotation    = XMMatrixRotationX(GROUND_ROTATION);
    const MATRIX4X4 mTranslation = XMMatrixTranslation(groundPos.x, groundPos.y, groundPos.z);
    const MATRIX4X4 mWorld       = mScale * mRotation * mTranslation;

    const MATRIX4X4 mView = GameDevice()->m_mView;
    const MATRIX4X4 mProj = GameDevice()->m_mProj;

    const DWORD texWidth  = m_pCircleImage->m_dwImageWidth;
    const DWORD texHeight = m_pCircleImage->m_dwImageHeight;

    spr.Draw3DWithWorldMatrix(
        m_pCircleImage,
        mWorld, mView, mProj,
        VECTOR2(SPRITE_SIZE, SPRITE_SIZE),
        VECTOR2(0, 0),
        VECTOR2(static_cast<float>(texWidth), static_cast<float>(texHeight)),
        CIRCLE_ALPHA
    );
}

///Debug///
void CConeDraw::DrawDebugCone() const
{
    CSprite spr;
    const float   radius    = m_pLevel->GetRadius();
    const float   coneTop   = m_pLevel->GetConeTopPos();
    const VECTOR3 playerPos = m_pPlayer->GetPos();
    const VECTOR3 center    = VECTOR3(playerPos.x, 0, playerPos.z);

    constexpr int   segments  = 32;
    const     float angleStep = 2.0f * 3.14159f / segments;

    for (int i = 0; i < segments; ++i)
    {
        float   angle1 = static_cast<float>(i) * angleStep;
        VECTOR3 point1 = center;
        point1.x += radius * cos(angle1);
        point1.z += radius * sin(angle1);

        float   angle2 = (i + 1) % segments * angleStep;
        VECTOR3 point2 = center;
        point2.x += radius * cos(angle2);
        point2.z += radius * sin(angle2);

        spr.DrawLine3D(point1, point2, RGB(0, 255, 0));
    }

    const VECTOR3 topPos = VECTOR3(playerPos.x, coneTop, playerPos.z);
    spr.DrawLine3D(topPos, VECTOR3(center.x + radius, center.y, center.z), RGB(0, 255, 0));
    spr.DrawLine3D(topPos, VECTOR3(center.x - radius, center.y, center.z), RGB(0, 255, 0));
    spr.DrawLine3D(topPos, VECTOR3(center.x, center.y, center.z + radius), RGB(0, 255, 0));
    spr.DrawLine3D(topPos, VECTOR3(center.x, center.y, center.z - radius), RGB(0, 255, 0));
}