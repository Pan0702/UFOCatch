#include "Dog.h"

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"

#include "../System/EnemyManager.h"
#include "../Base/StateBase.h"
#include "../../Utils/BBox.h"
#include "../Component/Idle.h"
#include "../Component/Walk.h"
#include "../Component/Suction.h"
#include "../Component/Destroy.h"
#include "../../Utils/MyLib.h"
#include "../../Common/ShadowObject.h"

CADog::CADog(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_areaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();
    InitStates();
    Instantiate< CShadowObject>(this, TEXT("data/CircleSuction.png"));
    m_pBBox = CreateBBox();
}

void CADog::InitStates()
{
    //繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譛邨ゅヵ繝ｬ繝ｼ繝 : 570.0f //
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this, 570.0f);      
    //遘ｻ蜍輔せ繝斐・繝・: 1.2f//
    m_components[CBaseState::State::WALK] = std::make_unique< CWalk>(this, 1.2f);
    m_components[CBaseState::State::SUCTION] = std::make_unique<CSuction>(this);
    //Score : 100 ,Exp : 1.0f //
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroy>(this, 100, 1.0f);
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState = std::make_unique< CBaseState>(this);
    m_pState->Enter(CBaseState::State::IDLE);
}

CADog::~CADog()
{
}


void CADog::Update()
{
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    //蝨ｰ髱｢縺ｨ縺ｮ蠖薙◆繧雁愛螳壹→縲，omponent縺ｮUpdate繧貞他縺ｳ蜃ｺ縺励※繧・
    CEnemyBase::Update();

}

VECTOR3 CADog::SuctionSpeed() const
{
    constexpr float suctionTime = 1.0f;
    return m_pPlayer->CalcSuctionDisplacement(suctionTime,transform.position);
}


bool CADog::ShouldApplyGravity() const
{
  return m_pComponent != m_components.at(CBaseState::State::SUCTION).get();
}

