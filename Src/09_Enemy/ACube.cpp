#include "ACube.h"

#include "../08_Player/Player.h"
#include <chrono>
#include <thread>

CACube::CACube()
{
    m_pWhiteMesh = new CFbxMesh();
    m_pWhiteColl = new MeshCollider();
    m_pWhiteMesh->Load("data/LowPoly/white.mesh");
    m_pWhiteColl->MakeFromMesh(m_pWhiteMesh);

    m_pRedMesh = new CFbxMesh();
    m_pRedColl = new MeshCollider();
    m_pRedMesh->Load("data/LowPoly/Red1.mesh");
    m_pRedColl->MakeFromMesh(m_pRedMesh);

    transform.position = VECTOR3(0, 0, 0);
    m_maxSize = CAnimalManager::GetObjectSize(m_pRedColl);
    m_isMovingToUFO = false;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    num = 0;
    timeReset = false;
}

void CACube::Init()
{
    auto = [](CFbxMesh* mesh,MeshCollider coll,const char* name)
    {
        
    };
    
}


CACube::~CACube()
{
    DeletePtr(m_pWhiteMesh);
    DeletePtr(m_pWhiteColl);
    DeletePtr(m_pRedMesh);
    DeletePtr(m_pRedColl);
    DeletePtr(m_pPlayer);
    DeletePtr(m_pCurentState);
}


void CACube::Update()
{
    m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position + VECTOR3(0, m_maxSize.y, 0));

    if (m_isInConeArea && m_pPlayer->GetIsSuckUp())
    {
        
    }
    ImGui::Begin("ACube");
    ImGui::Text("transform.position.z:%lf", transform.position.z);
    ImGui::Text("transform.Rotate.y:%lf", transform.rotation.y * RadToDeg);
    ImGui::Text("timer:%lf", time);
    ImGui::End();
    if (m_pCurentState) {
        m_pCurentState->Update(*this);
    }
}


void CACube::Draw()
{
    if (m_isInConeArea)
    {
        RedDraw();
    }
    else
    {
        WhiteDraw();
    }
}

void CACube::WhiteDraw()
{
    m_pWhiteMesh->Render(transform.matrix());
}

void CACube::RedDraw()
{
    m_pRedMesh->Render(transform.matrix());
}

void CACube::EnemyMove()
{
    switch (num)
    {
    case 0:
        {
            if (!timeReset)
            {
                TimerInit();
            }
            MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y);
            transform.position = transform.position + VECTOR3(0, 0, moveAmount * SceneManager::DeltaTime()) * mat;
            time += SceneManager::DeltaTime();
            if (time >= 5.0f)
            {
                num = 1;
                timeReset = false;
            }
            break;
        }
    case 1:
        {
            if (!timeReset)
            {
                TimerInit();
                // 回転開始時の角度を記録
                startRotationY = transform.rotation.y;
            }
            
            // 経過時間に応じて目標角度まで線形補間
            float progress = time / 1.0f; // 1秒で完了
            const float ANGLE_90 = 90.0f * DegToRad;
            if (progress >= 1.0f)
            {
                progress = 1.0f;
                // 正確に90度回転した状態で終了
                transform.rotation.y = startRotationY + ANGLE_90;
                num = 0;
                timeReset = false;
            }
            else
            {
                // 線形補間で正確な角度を設定
                transform.rotation.y = startRotationY + (ANGLE_90 * progress);
            }
            
            time += SceneManager::DeltaTime();
            break;
        }
    }
}
void CACube::EnemySuction()
{
    m_distanceFromObjectToUFO = m_pPlayer->
        CalcSuctionVelocity(100, transform.position + VECTOR3(0, m_maxSize.y, 0));
    MoveForUFO(transform.position, m_distanceFromObjectToUFO, 10);
}


void CACube::MoveForUFO(const VECTOR3& animalPos, const VECTOR3& distanceFromObjectToUFO, const int& exp)
{
    if (m_pPlayer->GetPos().y <= animalPos.y)
    {
        
    }
    else
    {
        transform.position += distanceFromObjectToUFO;
    }
}

void CACube::EnemyDestry()
{
    m_pPlayer->AddExp(1);
    DestroyMe();
}

void CACube::TimerInit()
{
    time = 0.0f;
    timeReset = true;
}

void CACube::SetState(IACubeState* newState)
{
    // C#の「currentState?.Exit(this);」に相当
    // currentState が null でない場合に Exit を呼ぶ
    if (m_pCurentState) {
        // メソッドに渡すのはポインタ(this)ではなく参照(*this)
        m_pCurentState->Exit(*this);
    }

    // C#の「currentState = newState;」に相当
    // newState の所有権を currentState に移動
    m_pCurentState = std::move(newState);

    // C#の「currentState.Enter(this);」に相当
    // (nullチェックを入れるのがより安全)
    if (m_pCurentState) {
        m_pCurentState->Enter(*this);
    }
}
