#include "Player.h"

#include "PCamera.h"


CPlayer::CPlayer()
{
    transform.position = VECTOR3(0, 10, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Mousey/Mousey.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(0, "data/Mousey/Anim_Run.anmx", true);
    m_pAnimator->Play(0);

    m_coneDegree = 23;
    m_coneRadius = 10 * tan(DegToRad * m_coneDegree);
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
    ImGui::Text("Pos.x%lf", transform.position.x);
    ImGui::Text("Pos.z%lf", transform.position.z);
    ImGui::End();
}

void CPlayer::Draw()
{
    if (m_pAnimator == nullptr)
    {
        m_pMesh->Render(transform.matrix());
    }
    else
    {
        m_pMesh->Render(m_pAnimator, transform.matrix());
    }
    DrawCircle(VECTOR3(transform.position.x, 0, transform.position.z), m_coneRadius, RGB(0, 255, 0));
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

void CPlayer::SuckUpAnimal()
{
    
}

bool CPlayer::IsInConeArea(const VECTOR3& pos)
{
    const float distnceAnimalFromPlayer = transform.position.y - pos.y;
    const float animalPositionRadius = distnceAnimalFromPlayer * std::tan(DegToRad * m_coneDegree);
    if (std::pow((pos.x - transform.position.x), 2) + std::pow((pos.z - transform.position.z), 2)
        <= std::pow(animalPositionRadius, 2))
    {
        return true;
    }
    return false;
}

void CPlayer::DrawCircle(const VECTOR3& center, float radius, DWORD color)
{
    CSprite spr;
    const int segments = 32; // 円を構成する線分の数（多いほど滑らかな円になる）
    const float angleStep = 2.0f * 3.14159f / segments; // 各線分の角度

    for (int i = 0; i < segments; ++i)
    {
        // 現在の点
        float angle1 = i * angleStep;
        VECTOR3 point1 = center;
        point1.x += radius * cos(angle1);
        point1.z += radius * sin(angle1);

        // 次の点
        float angle2 = ((i + 1) % segments) * angleStep;
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
