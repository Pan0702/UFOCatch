#include "Human.h"

#include "FunShape.h"
#include "../../08_Player/PHP.h"
#include "../../11_GameSystem/VisionSystem.h"
#include "../State/HumanState.h"

namespace
{
    constexpr float LINE_LENGTH = 7.0f;
}

CHuman::CHuman(VECTOR3 pos, VECTOR2 areaSize)
    : m_AreaSize(areaSize)
{
    transform.position = pos;
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Human/Human.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);
    m_pMesh->LoadAnimation(A_WALK, "data/NewAnimal/Human/Human_Walk.anmx", true);
    m_pMesh->LoadAnimation(A_SEACH, "data/NewAnimal/Human/Human_Find.anmx", false);

    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_dwColor = 100;
    angle = 0.0f;

    m_cubeStates[CBaseState::Type::IDLE] = new CHumanIdleState(this);
    m_cubeStates[CBaseState::Type::WALK] = new CHumanWalkState(this);
    m_cubeStates[CBaseState::Type::FIND_PLAYER] = new CHumanFindPlayer(this);
    m_pCurrentState = m_cubeStates[CBaseState::Type::IDLE];
    m_pCurrentState->Enter();
    m_pCurrentState->SetNextState();
    m_pFunShape = new CFunShape();
}

CHuman::~CHuman()
{
    for (auto& state : m_cubeStates)
    {
        if (state.second == nullptr) continue;
        SAFE_DELETE(state.second);
        state.second = nullptr;
    }
    m_cubeStates.clear();
}

void CHuman::Update()
{
    m_pAnimator->Update();
    CVisionSystem*vision = ObjectManager::FindGameObject<CVisionSystem>();
    m_inSight = vision->SectorCircleCollision(ToVec2XZ(transform.position), transform.rotation.y)
        && ObjectManager::FindGameObject<CPlayer>()->GetIsSuckUp();

    if (m_pCurrentState)
    {
        m_pCurrentState->Update();
    }
    if (m_inSight)
    {
        m_dwColor = 255;
        ChangeState(CBaseState::Type::FIND_PLAYER);
    }
    else
    {
        m_dwColor = 0;
        ObjectManager::FindGameObject<CPlayerHP>()->ResetFlag();
    }
    AtkArea();
}


void CHuman::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
    DrawDirectionLine();
    //FanShape();
}

void CHuman::ChangeState(CBaseState::Type type)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_cubeStates[type];
    m_pCurrentState->Enter();
}

void CHuman::AtkArea() const
{
    m_pFunShape->PosSet(transform.position, angle + transform.rotation.y);
}

//Humanの範囲をLineで可視化
//範囲内なら水色、外なら緑になる
void CHuman::DrawDirectionLine()
{
    CSprite spr;
    MATRIX4X4 mat = XMMatrixRotationY(angle + transform.rotation.y);

    VECTOR3 startPos = transform.position;

    VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;

    spr.DrawLine3D(startPos, endPos, RGB(0, 255, m_dwColor));
}


// void CHuman::FanShape()
// {
//     CSprite spr;
//     
//     float angle = -(20.0f * DegToRad);
//     
//     for (int i = 0; i < 3; i++)
//     {
//         if (i == 2) angle = std::abs(angle);
//
//         MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y + angle);
//         
//         VECTOR3 startPos = transform.position;
//         VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;
//
//         spr.DrawLine3D(startPos, endPos, RGB(255, 0, 0));
//     }
// }
