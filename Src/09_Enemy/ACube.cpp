#include "ACube.h"

#include "../08_Player/Player.h"
#include <chrono>
#include <queue>
#include <thread>

namespace
{
    std::queue<IACubeState*> actionQueue;
    std::random_device rd;
    std::mt19937 gen(rd());
}

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
    m_isMovingToUFO = false;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();

    SetNextState();
    SetState(new CMoveState());
}

CACube::~CACube()
{
    SAFE_DELETE(m_pWhiteMesh);
    SAFE_DELETE(m_pRedMesh);
    SAFE_DELETE(m_pWhiteColl);
    SAFE_DELETE(m_pRedColl);
    SAFE_DELETE(m_pCurentState);
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
    if (m_pCurentState)
    {
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


template <class C>
void CACube::DrawObject(C c)
{
    c->Render(transform.matrix());
}


void CACube::SetState(IACubeState* newState)
{
    if (m_pCurentState)
    {
        m_pCurentState->Exit(*this);
    }
    SAFE_DELETE(m_pCurentState);
    m_pCurentState = std::move(newState);

    while (actionQueue.size() <= 3)
    {
        SetNextState();
    }

    if (m_pCurentState)
    {
        m_pCurentState->Enter(*this);
    }
}

void CACube::InitList()
{
}

void CACube::SetNextState()
{
    float randomNum = MyRamdom(0, 1, gen);
    if (randomNum < 0.5f)
    {
        actionQueue.push(new CStopState());
    }
    else
    {
        actionQueue.push(new CMoveState());
    }
}

void CACube::Destroy()
{
    DestroyMe();
}

void CStopState::Update(CACube& cube)
{
    number++;
    if (number > 50)
    {
        IACubeState* nextAction = actionQueue.front();
        actionQueue.pop();
        cube.SetState(nextAction);
    }
}

void CMoveState::Enter(CACube& cube)
{
    const float MAX_MOVE_SPEED = 2.0f;
    const float MIN_MOVE_SPEED = 0.5f;
    const float MAX_MOVE_AMOUNT = 3.5f;
    const float MIN_MOVE_AMOUNT = 1.0f;
    const float TURN_ANGLE = 180.0f;
    m_totalPosZMoveAmount = 0;

    m_moveSpeed = MyRamdom(MIN_MOVE_SPEED, MAX_MOVE_SPEED, gen);
    m_turnAmount = MyRamdom(-TURN_ANGLE, TURN_ANGLE, gen);
    m_moveAmount = MyRamdom(MIN_MOVE_AMOUNT, MAX_MOVE_AMOUNT, gen);
    m_savePos = cube.GetPos();
    cube.SetRotationY(m_turnAmount * DegToRad);
}

void CMoveState::Update(CACube& cube)
{
    {
        cube.AddPos(
            VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(m_turnAmount * DegToRad));
        m_totalPosZMoveAmount += m_moveSpeed * SceneManager::DeltaTime();

        ImGui::Begin("a");
        ImGui::Text("m_moveSpeed:%lf", m_moveSpeed);
        ImGui::Text("m_turnAmount:%lf", m_turnAmount);
        ImGui::Text("m_moveAmount:%lf", m_moveAmount);
        ImGui::End();
        if (m_totalPosZMoveAmount > m_moveAmount)
        {
            IACubeState* nextAction = actionQueue.front();
            actionQueue.pop();
            cube.SetState(nextAction);
        }
    }
}

void CMoveState::Exit(CACube& cube)
{
}
