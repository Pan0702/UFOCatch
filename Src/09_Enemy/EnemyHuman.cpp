#include "EnemyHuman.h"

namespace
{
    constexpr float LINE_LENGTH = 3.0f;
}
CEnemyHuman::CEnemyHuman()
{
    transform.position = VECTOR3(0, 0, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Mousey/Mousey.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(0, "data/Mousey/Anim_Run.anmx", true);
    m_pAnimator->Play(0);
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    
}

CEnemyHuman::~CEnemyHuman()
{
}

void CEnemyHuman::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_L))
    {
        transform.rotation.y += XM_PI / 12;
    }
    
}

void CEnemyHuman::Draw()
{
    Object3D::Draw();
    DrawDirectionLine();
    FanShape();
}

void CEnemyHuman::DrawDirectionLine()
{
    CSprite spr;
    MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y);

    VECTOR3 startPos = transform.position;
    
    VECTOR3 endPos = startPos + VECTOR3(LINE_LENGTH,0,LINE_LENGTH) * mat;

    spr.DrawLine3D(startPos,endPos,RGB(0,200,0));
    
}

void CEnemyHuman::FanShape()
{
    CSprite spr;
    
    float angle = -( XM_PI / 6);
    
    for (int i = 0;i < 3;i++)
    {
        if (i == 2)angle = std::abs(angle);
        float dirX = sin(transform.rotation.y + angle);
        float dirZ = cos(transform.rotation.y + angle);

        VECTOR3 startPos = transform.position;
    
        VECTOR3 endPos = VECTOR3(
            startPos.x + dirX * LINE_LENGTH,
            startPos.y,
            startPos.z + dirZ * LINE_LENGTH);

        spr.DrawLine3D(startPos,endPos,RGB(255,0,0));
    }
}
