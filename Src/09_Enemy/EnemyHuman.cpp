#include "EnemyHuman.h"

namespace
{
    constexpr float LINE_LENGTH = 7.0f;
}
CEnemyHuman::CEnemyHuman()
{
    transform.position = VECTOR3(0, 0, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Human/Human.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh); 
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);
    m_pMesh->LoadAnimation(A_WALK, "data/NewAnimal/Human/Human_Walk.anmx", true);
    m_pMesh->LoadAnimation(A_FIND, "data/NewAnimal/Human/Human_Find.anmx", false);
  
    m_pAnimator->Play(A_WALK);
    m_pAnimator->SetPlaySpeed(1.0f);
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_dwColor = 100;
    
}

CEnemyHuman::~CEnemyHuman()
{
}

void CEnemyHuman::Update()
{
    m_pAnimator->Update();
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_L))
    {
        transform.rotation.y += XM_PI / 12;
    }
    if (m_pPlayer->IsTargetInVidionFan(transform.rotation.y,transform.position))
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
    m_pMesh->Render(m_pAnimator, transform.matrix());
    DrawDirectionLine();
    FanShape();
}

void CEnemyHuman::DrawDirectionLine()
{
    CSprite spr;
    MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y);

    VECTOR3 startPos = transform.position;
    
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
        
        // �s����g���ē���
        MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y + angle);
        
        VECTOR3 startPos = transform.position;
        VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;

        spr.DrawLine3D(startPos, endPos, RGB(255, 0, 0));
    }
}