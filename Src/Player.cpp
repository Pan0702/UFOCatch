#include "Player.h"

#include "PCamera.h"


CPlayer::CPlayer()
{
    transform.position = VECTOR3(0, 0, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Mousey/Mousey.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(0,"data/Mousey/Anim_Run.anmx", true);
    m_pAnimator->Play(0);

    
}

CPlayer::~CPlayer()
{
    if (m_pMesh)
    {
        delete m_pMesh;
        m_pMesh = nullptr;
    }
    if (m_pAnimator)
    {
        delete m_pAnimator;
        m_pAnimator = nullptr;
    }
    
}

void CPlayer::Update()
{
    m_pAnimator->Update();
    PlayerMove();
    ImGui::Begin("Player");
    ImGui::Text("Pos.x%lf",transform.position.x);
    ImGui::Text("Pos.z%lf",transform.position.z);
    ImGui::End();
}

void CPlayer::Draw()
{
    if (m_pAnimator == nullptr)
    {
        m_pMesh->Render(transform.matrix());
    }
    else {
        m_pMesh->Render(m_pAnimator, transform.matrix());
    }
}

void CPlayer::PlayerMove()
{
    constexpr float moveSpeed = 0.01f;
    auto* input = GameDevice()->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) transform.position.z += moveSpeed;
    if (input->CheckKey(KD_DAT, DIK_S)) transform.position.z -= moveSpeed;
    if (input->CheckKey(KD_DAT, DIK_A)) transform.position.x -= moveSpeed;
    if (input->CheckKey(KD_DAT, DIK_D)) transform.position.x += moveSpeed;
    ObjectManager::FindGameObject<CPlayerCamera>()->PosSet(transform.position);
}

