#include "ACube.h"

#include "../08_Player/Player.h"
#include <queue>
#include <thread>

#include "SpatialGrid.h"

namespace
{
    std::queue<InterfaceACubeState*> actionQueue;
}

CACube::CACube(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_moveAreaSize(moveAreaSize)
{
    m_pWhiteMesh = new CFbxMesh();
    m_pWhiteColl = new MeshCollider();
    m_pWhiteMesh->Load("data/LowPoly/white.mesh");
    m_pWhiteColl->MakeFromMesh(m_pWhiteMesh);

    m_pRedMesh = new CFbxMesh();
    m_pRedColl = new MeshCollider();
    m_pRedMesh->Load("data/LowPoly/Red1.mesh");
    m_pRedColl->MakeFromMesh(m_pRedMesh);

    transform.position = iniPos;
    m_maxSize = m_pRedColl->bBox.max;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGrid = ObjectManager::FindGameObject<SpatialGrid>();
    

    SetNextState();
    SetState(new CRunState());
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

    ImGui::Begin("ACube");
    ImGui::Text("transform.position.z:%lf", transform.position.z);
    ImGui::Text("transform.Rotate.y:%lf", transform.rotation.y * RadToDeg);
    ImGui::Text("timer:%lf", time);
    ImGui::End();
    if (m_pCurentState)
    {
        m_pCurentState->Update(*this);
    }
    m_pGrid->Insert(this);
    
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

//Stateをここでセット
void CACube::SetState(InterfaceACubeState* newState)
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

//次の行動をコンテナで管理
void CACube::SetNextState()
{
    float randomNum = Randomf(0, 1);
    if (randomNum < 0.5f)
    {
        actionQueue.push(new CIdleState());
    }
    else
    {
        actionQueue.push(new CRunState());
    }
}

void CACube::HitCheck()
{
   
    
    std::vector<CACube*> nearby = m_pGrid->CheckNearby(this);
    for (auto* cube : nearby)
    {
        if (cube == this)continue;
        
    }
}

void CACube::IsSuctionCheck()
{
    if (m_pPlayer->IsWithSuctionCone(transform.position + VECTOR3(0, m_maxSize.y, 0)) && m_pPlayer->GetIsSuckUp())
    {
        SetState(new CSuctionState);
    }
}

VECTOR3 CACube::SuctionSpeed()
{
    return m_pPlayer->
        CalcSuctionVelocity(100, transform.position + VECTOR3(0, m_maxSize.y, 0));
}

void CACube::Destroy()
{
    SAFE_DELETE(m_pCurentState);
    DestroyMe();
}

/////////////////////////////////////////////////////////////////
///Idle
/////////////////////////////////////////////////////////////////

void CIdleState::Update(CACube& cube)
{
    number++;
    if (number > 50)
    {
        cube.SetState(new CRunState());
        InterfaceACubeState* nextAction = actionQueue.front();
        actionQueue.pop();
        cube.SetState(nextAction);
    }
    cube.IsSuctionCheck();
}

/////////////////////////////////////////////////////////////////
///Run
/////////////////////////////////////////////////////////////////
void CRunState::Enter(CACube& cube)
{
    constexpr float MAX_MOVE_SPEED = 2.0f;
    constexpr float MIN_MOVE_SPEED = 0.5f;
    constexpr float MAX_MOVE_AMOUNT = 3.5f;
    constexpr float MIN_MOVE_AMOUNT = 1.0f;
    constexpr float TURN_ANGLE = 180.0f;
    m_totalPosZMoveAmount = 0;
    bool boundaryFlag = false;
    while (!boundaryFlag)
    {
        m_moveSpeed = Randomf(MIN_MOVE_SPEED, MAX_MOVE_SPEED);
        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
        m_moveAmount = Randomf(MIN_MOVE_AMOUNT, MAX_MOVE_AMOUNT);
        m_position = cube.GetPos();
        if (boundaryCheck(cube.MoveAreaSize()))
        {
            boundaryFlag = true;
        }
    }
    cube.SetRotationY(m_turnAmount);
}

void CRunState::Update(CACube& cube)
{
    cube.AddPos(
        VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(m_turnAmount));
    m_totalPosZMoveAmount += m_moveSpeed * SceneManager::DeltaTime();

    ImGui::Begin("a");
    ImGui::Text("m_moveSpeed:%lf", m_moveSpeed);
    ImGui::Text("m_turnAmount:%lf", m_turnAmount);
    ImGui::Text("m_moveAmount:%lf", m_moveAmount);
    ImGui::End();
    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        cube.SetState(new CIdleState());
        InterfaceACubeState* nextAction = actionQueue.front();
        actionQueue.pop();
        cube.SetState(nextAction);
    }
    cube.IsSuctionCheck();
}

bool CRunState::boundaryCheck(const VECTOR2& areaSize)
{
    VECTOR3 tmpPos = m_position + VECTOR3(0, 0, m_moveAmount) * XMMatrixRotationY(m_turnAmount);
    if (tmpPos.x <= areaSize.x && tmpPos.x >= -areaSize.x && tmpPos.z <= areaSize.y && tmpPos.z >= -areaSize.y)
    {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////
///Suction
/////////////////////////////////////////////////////////////////
void CSuctionState::Enter(CACube& cube)
{
    MeshCollider* meshColl = new MeshCollider();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_distanceFromObjectToUFO = cube.SuctionSpeed();
    SAFE_DELETE(meshColl);
}

void CSuctionState::Update(CACube& cube)
{
    if (m_pPlayer->GetIsSuckUp())
    {
        if (m_pPlayer->GetPos().y <= cube.GetPos().y)
        {
            cube.SetState(new CDestoryState());
        }
        else
        {
            cube.AddPos(m_distanceFromObjectToUFO);
        }
    }
    else
    {
        cube.SetState(new CIdleState());
    }
}

void CDestoryState::Enter(CACube& cube)
{
    cube.DestroyMe();
}
