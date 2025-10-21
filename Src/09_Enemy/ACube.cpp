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
   // Init();
    m_isMovingToUFO = false;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    num = 0;
    timeReset = false;
    SetState(new CMoveState());
}

void CACube::Init()
{
    auto Init = [](CFbxMesh* mesh,MeshCollider* coll,const char* name)
    {
        mesh = new CFbxMesh();
        coll = new MeshCollider();
        mesh->Load(name);
        coll->MakeFromMesh(mesh);
    };
    Init(m_pWhiteMesh,m_pWhiteColl,"data/LowPoly/white.mesh");
    Init(m_pRedMesh,m_pRedColl,"data/LowPoly/Red1.mesh");
}


CACube::~CACube()
{
    DeletePtr(m_pWhiteMesh);
    DeletePtr(m_pRedMesh);
    DeletePtr(m_pWhiteColl);
    DeletePtr(m_pRedColl);
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
        DrawObject(m_pRedMesh);
    }
    else
    {
        DrawObject(m_pWhiteMesh);
    }
}



template<class C>
void CACube::DrawObject(C c)
{
    c->Render(transform.matrix());
}


void CACube::SetState(IACubeState* newState)
{
    if (m_pCurentState) {
        m_pCurentState->Exit(*this);
    }
    m_pCurentState = std::move(newState);
    
    if (m_pCurentState) {
        m_pCurentState->Enter(*this);
    }
}

void CStopState::Update(CACube& cube)
{
    number++;
    if (number > 10)
    {
        cube.SetState(new CMoveState());
    }
}

void CMoveState::Enter(CACube& cube)
{
    const float MAX_MOVE_SPEED = 3.0f;
    const float MIN_MOVE_SPEED = 0.5f;
    const float MAX_MOVE_AMOUNT = 5.0f;
    const float MIN_MOVE_AMOUNT = 1.0f;
    const float TURN_ANGLE = 150.0f;
    m_totalPosZMoveAmount = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(MIN_MOVE_SPEED, MAX_MOVE_SPEED);
    m_moveSpeed = dist(gen);
    std::uniform_real_distribution<float> dist2(-TURN_ANGLE, TURN_ANGLE);
    m_turnAmount = dist2(gen);
    std::uniform_real_distribution<float> dist3(MIN_MOVE_AMOUNT,MAX_MOVE_AMOUNT );
    m_moveAmount = dist3(gen);
    m_savePos = cube.GetPos();
    cube.SetRotationY(m_turnAmount * DegToRad);
}

void CMoveState::Update(CACube& cube)
{
    {
        cube.AddPos(VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(m_turnAmount * DegToRad));
        m_totalPosZMoveAmount += m_moveSpeed * SceneManager::DeltaTime();

        if (m_totalPosZMoveAmount > m_moveAmount)
        {
            cube.SetState(new CStopState());
        }
    }
}
