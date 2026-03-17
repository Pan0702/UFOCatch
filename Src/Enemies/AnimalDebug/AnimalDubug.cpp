#include "AnimalDubug.h"

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"
#include "../Base/StateBase.h"
#include "../Component/Idle.h"
#include "../Component/Suction.h"
#include "../Component/Walk.h"
#include "../Component/Destroy.h"
#include "../System/EnemyManager.h"
#include "../../Utils/MyLib.h"

CADebug::CADebug(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    m_areaSize = moveAreaSize;
    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();
    
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this,570.0f);
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this, 2.0f);
    m_components[CBaseState::State::SUCTION] = std::make_unique<CSuction>(this);
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroy>(this,100,1);
    m_pState->Enter(CBaseState::State::WALK);
    m_pBBox = CreateBBox();
}

CADebug::~CADebug() = default;


void CADebug::Update()
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    CEnemyBase::Update();

    // 蜑企勁繝輔Λ繧ｰ縺檎ｫ九▲縺ｦ縺・ｋ・・EnemyBase::Update縺ｧ蜃ｦ逅・′繧ｹ繧ｭ繝・・縺輔ｌ縺滂ｼ牙ｴ蜷医・縲・
    // 縺薙ｌ莉･荳翫・蜃ｦ逅・ｼ郁｡晉ｪ∝愛螳壹↑縺ｩ・峨ｒ陦後ｏ縺ｪ縺・
    if (m_pState != nullptr && m_pComponent == m_components[CBaseState::State::DESTROY].get())
    {
        return;
    }

    m_pAnimator->Update();
    ResolveOBBCollisions();
    UpdateBBox();

    // 繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ陦晉ｪ∝愛螳壹→謚ｼ縺玲綾縺暦ｼ域怙蠕後↓螳溯｡鯉ｼ・
    ResolveStageCollisions();
}

void CADebug::Draw()
{
    m_pMesh->Render(m_pAnimator.get(), transform.matrix());
}

VECTOR3 CADebug::SuctionSpeed() const
{
    constexpr float suctionTime = 1.0f;
    return m_pPlayer->CalcSuctionDisplacement(suctionTime,transform.position);
}

bool CADebug::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::State::SUCTION).get();
}
