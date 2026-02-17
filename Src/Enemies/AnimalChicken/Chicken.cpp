#include "Chicken.h"

#include <iostream>

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"
#include "../System/EnemyManager.h"
#include "../Base/StateBase.h"
#include "State/ChickenState.h"
#include "../../Utils/BBox.h"
#include "../Component/Idle.h"
#include "../Component/Suction.h"
#include "../Component/Walk.h"
#include "../Component/Destroy.h"
#include "../../Utils/MyLib.h"  

CAnimalChicken::CAnimalChicken(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_areaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Chicken");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();

    m_components[CBaseState::State::IDLE] = new CIdle(this,360.0f);
    m_components[CBaseState::State::WALK] = new CWalk(this, 1.2f);
    m_components[CBaseState::State::SUCTION] = new CSuction(this);
    m_components[CBaseState::State::DESTROY] = new CDestroy(this,150,1.5f);
    m_pComponent = m_components[CBaseState::State::WALK];
    m_pState = new CBaseState(this);
    m_pState->Enter(CBaseState::State::WALK);
    m_pBBox = CreateBBox();
    m_pCry = new CXAudioSource(_T("data/Sound/ChickenCry.wav"));
    m_pSpriteImage = new CSpriteImage(TEXT("data/CircleSuction.png"));
}

CAnimalChicken::~CAnimalChicken()
{
    for (auto& state : m_components)
    {
        SAFE_DELETE(state.second);
    }
    SAFE_DELETE(m_pCry);
    SAFE_DELETE(m_pSpriteImage);
}

void CAnimalChicken::Update()
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    CEnemyBase::Update();

    if (m_pState != nullptr && m_pComponent == m_components[CBaseState::State::DESTROY])
    {
        return;
    }
    if (m_pAnimator != nullptr) // アニメーターが存在するかチェック
    {
        m_pAnimator->Update();
    }
    ResolveOBBCollisions();
    UpdateBBox();

    ResolveStageCollisions();
}

void CAnimalChicken::Draw()
{
    Object3D::Draw();
}

void CAnimalChicken::IsSuctionCheck()
{
    if (m_pPlayer == nullptr)return;
    if (m_pPlayer->IsWithSuctionCone(transform.position) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CBaseState::State::SUCTION);
    }
}

const VECTOR3& CAnimalChicken::SuctionSpeed() const
{
    constexpr float suctionTime = 1.0f;
    return m_pPlayer->CalcSuctionDisplacement(suctionTime,transform.position);
}

bool CAnimalChicken::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::State::SUCTION);
}