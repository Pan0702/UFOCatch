#include "ACube.h"

#include "../08_Player/Player.h"
#include <queue>
#include <thread>

#include "AnimalManager.h"
#include "AnimalManager.h"
#include "SpatialGrid.h"
#include "State/ACubeState.h"

CACube::CACube(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_moveAreaSize(moveAreaSize)
{
    m_pMesh = new CFbxMesh();
    m_pAnimator = new Animator();
    m_pMesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(AnimationType::A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);
    m_pMesh->LoadAnimation(AnimationType::A_WALK, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
    m_pMesh->LoadAnimation(AnimationType::A_RUN, "data/NewAnimal/Dog/Dog_Idle.anmx", true);

    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();

    m_cubeStates[CACubeState::Type::Idle] = new CIdleState(this);
    m_cubeStates[CACubeState::Type::Walk] = new CWalkState(this);
    m_cubeStates[CACubeState::Type::Suction] = new CSuction(this);
    m_cubeStates[CACubeState::Type::Destroy] = new CDestroy(this);
    m_pCubeState = m_cubeStates[CACubeState::Type::Idle];
    m_pCubeState->SetNextState();
}

CACube::~CACube()
{
    for (auto& state : m_cubeStates)
    {
        SAFE_DELETE(state.second);
    }
}


void CACube::Update()
{
    m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    m_pAnimator->Update();
    if (m_pCubeState)
    {
        m_pCubeState->Update();
    }
    ImGui::Begin("begin");
    ImGui::Text("CurrentAnim: %lf", transform.rotation.y * RadToDeg);
    ImGui::End();
}


//Stateをここでセット
void CACube::SetState(CACubeState::Type type)
{
    m_pCubeState->Exit();
    m_pCubeState = m_cubeStates[type];
    m_pCubeState->Enter();
}

void CACube::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
}

void CACube::IsSuctionCheck()
{
    if (m_pPlayer->IsWithSuctionCone(transform.position /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CACubeState::Type::Suction);
    }
}

VECTOR3 CACube::SuctionSpeed() const
{
    return m_pPlayer->
        CalcSuctionVelocity(100, transform.position);
}

void CACube::SetAnim(const int& animNum) const
{
    m_pAnimator->MergePlay(animNum);
}

bool CACube::AnimationFinish() const
{
    if (m_pAnimator->CurrentFrame() >= 570.0f)
    {
        return true;
    }
    return false;
}

void CACube::SetRotationY(const float& angle)
{
    float degAngle = angle * RadToDeg;

    // -180 ~ 180度の範囲に正規化
    while (degAngle > 180.0f)
    {
        degAngle -= 360.0f;
    }
    while (degAngle < -180.0f)
    {
        degAngle += 360.0f;
    }

    transform.rotation.y = degAngle * DegToRad;
}
