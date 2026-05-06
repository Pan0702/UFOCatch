#include "Chicken.h"

#include <iostream>

#include "State/ChickenState.h"

#include "../../Common/ShadowObject.h"
#include "../Human/Human.h"
#include "../Component/ComponentFwd.h"

CAnimalChicken::CAnimalChicken(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_areaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Chicken");
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();


    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this, 360.0f);
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this, 1.2f);
    m_components[CBaseState::State::SUCTION] = std::make_unique<CSuction>(this);
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroy>(this, 200, 1.5f);
    m_pComponent = m_components[CBaseState::State::WALK].get();
    m_pState = std::make_unique<CBaseState>(this);
    m_pState->Enter(CBaseState::State::WALK);
    m_pBBox = CreateBBox();
    m_pCry = new CXAudioSource(_T("data/Sound/ChickenCry.wav"));
    new CShadowObject(this, TEXT("data/CircleSuction.png"));
}

CAnimalChicken::~CAnimalChicken()
{
    SAFE_DELETE(m_pCry);
}


void CAnimalChicken::Cry() const
{
    m_pCry->Play();
    // 距離5以下のHumanをチキンの方向に向かせる
    auto humans = ObjectManager::FindGameObjects<CHuman>();
    for (auto* human : humans)
    {
        const VECTOR3 humanPos = human->GetTransform().position;
        const VECTOR3 dir = transform.position - humanPos;
        const float distance = dir.x * dir.x + dir.z * dir.z;
        if (distance <= 180.0f)
        {
            const float angle = atan2f(dir.x, dir.z);
            human->SetRotateY(angle);
        }
    }
}

void CAnimalChicken::Update()
{
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    CEnemyBase::Update();
}

void CAnimalChicken::ChangeState(CBaseState::State type)
{
    CEnemyBase::ChangeState(type);
    if (m_pComponent == m_components.at(CBaseState::State::SUCTION).get())
    {
        Cry();
    }
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
        ChangeState(CBaseState::State::SUCTION);
    }
}

VECTOR3 CAnimalChicken::SuctionSpeed() const
{
    constexpr float suctionTime = 1.0f;
    return m_pPlayer->CalcSuctionDisplacement(suctionTime, transform.position);
}

bool CAnimalChicken::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::State::SUCTION).get();
}
