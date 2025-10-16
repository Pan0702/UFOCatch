#include "EnemyHuman.h"

namespace
{
    constexpr float LINE_LENGTH = 7.0f;
}
CEnemyHuman::CEnemyHuman()
{
    transform.position = VECTOR3(0, 0, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Ghost/Ghost.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_dwColor = 100;
    
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
    if (m_pPlayer->IsHumanFieldOfVision(transform.rotation,20.0f,transform.position))
    {
        m_dwColor = 255;
    }
    else
    {
        m_dwColor = 0;
    }
    
}

void CEnemyHuman::Draw()
{
    m_pMesh->Render(transform.matrix());
    DrawDirectionLine();
    FanShape();

    
}

void CEnemyHuman::DrawDirectionLine()
{
    CSprite spr;
    MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y);

    VECTOR3 startPos = transform.position;
    
    // 緑が正しかったので、Z方向を使用
    VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;

    
    spr.DrawLine3D(startPos, endPos, RGB(0, 255, m_dwColor));
}

void CEnemyHuman::FanShape()
{
    CSprite spr;
    
    float angle = -(20.0f * DegToRad);
    
    for (int i = 0; i < 3; i++)
    {
        if (i == 2) angle = std::abs(angle);
        
        // 行列を使って統一
        MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y + angle);
        
        VECTOR3 startPos = transform.position;
        VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;

        spr.DrawLine3D(startPos, endPos, RGB(255, 0, 0));
    }
}