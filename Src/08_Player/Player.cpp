
#include "Player.h"
#include "PCamera.h"
#include <chrono>


#include "../06_GameLib/Lerp.h"


CPlayer::CPlayer()
{
    transform.position = VECTOR3(0, 10, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Mousey/Mousey.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(0, "data/Mousey/Anim_Run.anmx", true);
    m_pAnimator->Play(0);
    m_coneDegree = 20;
    m_allExp = 1;
    m_exp = 0;
    baseRadius = transform.position.y * tan(DegToRad * m_coneDegree);
    baseHeight = transform.position.y;
}

CPlayer::~CPlayer() = default;

void CPlayer::Update()
{
    
    m_pAnimator->Update();
    PlayerMove();
    
    // コーンの半径を計算
    m_coneRadius = transform.position.y * tan(DegToRad * m_coneDegree);
    
    ImGui::Begin("Player");
    ImGui::Text("Cone Radius: %lf", m_coneRadius);
    ImGui::Text("Cone Height: %lf", transform.position.y);
    ImGui::End();
    
    CheckLevel();
    
    // カメラ位置を更新
    CameraPos();

    m_isSuckUp = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_J);
}

void CPlayer::Draw()
{
    //DrawCircle(VECTOR3(transform.position.x, 0, transform.position.z), m_coneRadius, RGB(0, 255, 0));
    Object3D::Draw();
}

void CPlayer::PlayerMove()
{
    constexpr float moveSpeed = 0.1f;
    auto* input = GameDevice()->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) transform.position.z += moveSpeed;
    if (input->CheckKey(KD_DAT, DIK_S)) transform.position.z -= moveSpeed;
    if (input->CheckKey(KD_DAT, DIK_A)) transform.position.x -= moveSpeed;
    if (input->CheckKey(KD_DAT, DIK_D)) transform.position.x += moveSpeed;
}

void CPlayer::CheckLevel()
{
    if (m_exp >= m_allExp)
    {
        int tmp = m_exp - m_allExp;
        m_allExp *= 1.3;
        m_exp = tmp;
        IncreaseConeVertexHeight();
    }
}

void CPlayer::IncreaseConeVertexHeight()
{
    // 高さを徐々に増加させる
    for (float i = 0.1f; i <= 1.0f; i += 0.1f)
    {
        transform.position.y += 1.0f;
    }
    
    // 半径を更新
    m_coneRadius = transform.position.y * tan(DegToRad * m_coneDegree);
}

void CPlayer::CameraPos()
{
    // カメラ位置をコーンの高さに基づいて設定
    ObjectManager::FindGameObject<CPlayerCamera>()->PosSet(
        transform.position, transform.position.y);
}



//吸い込むスピードを計算
VECTOR3 CPlayer::SuckUpAnimal(const int& dividend, const VECTOR3& animalPos) const
{
    float k = (0 - animalPos.y) / (animalPos.y - transform.position.y);
    VECTOR3 distnceAnimalFromPlayer =
        VECTOR3(animalPos.x + k * (animalPos.x - transform.position.x), 0,
                animalPos.z + k * (animalPos.z - transform.position.z));
    distnceAnimalFromPlayer = transform.position - distnceAnimalFromPlayer;
    return distnceAnimalFromPlayer / dividend;
}


//エリア内にいるかチェック
bool CPlayer::IsInConeArea(const VECTOR3& pos) const
{
    const float distnceAnimalFromPlayer = transform.position.y - pos.y;
    const float animalPositionRadius = distnceAnimalFromPlayer * std::tan(DegToRad * m_coneDegree);
    if (Pow2(pos.x - transform.position.x) + Pow2(pos.z - transform.position.z)
        <= Pow2(animalPositionRadius))
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