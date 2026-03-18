#include "ShepherdDog.h"

#include "../Component/Idle.h"
#include "../System/Flog.h"
#include "../Component/SheperdDogComp.h"
#include "../System/EnemyManager.h"
#include "../Component/Walk.h"

CAShepherdDog::CAShepherdDog()
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    //蜍輔￥繧ｹ繝斐・繝・.0f
    m_components[CBaseState::State::COLLECTING] = std::make_unique<CCollecting>(this, 2.0f);
    //蜍輔￥繧ｹ繝斐・繝・.0f
    m_components[CBaseState::State::DRIVING] = std::make_unique<CDriving>(this, 2.0f);
    m_components[CBaseState::State::RESCUE] = std::make_unique<CRescue>(this);
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this, 570.0f);
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this, 1.2f);
    //繧ｹ繧ｳ繧｢縺鯉ｼ抵ｼ撰ｼ舌∫ｵ碁ｨ灘､縺・.0f
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroyShepherdDog>(this, 200, 2.0f);
    m_pState = std::make_unique<CBaseState>(this);
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState->Enter(CBaseState::State::IDLE);
    m_pBBox = CreateBBox();
}

CAShepherdDog::~CAShepherdDog() = default;

void CAShepherdDog::Update()
{
    // UFO縺悟精縺・ｾｼ縺ｿ荳ｭ縺九←縺・°繧偵Μ繧｢繝ｫ繧ｿ繧､繝縺ｧ遒ｺ隱・
    bool isSucking = false;
    if (m_pPlayer != nullptr && m_pPlayer->GetIsSuckUp())
    {
        for (const auto sheep : m_sheeps)
        {
            if (sheep != nullptr && m_pPlayer->IsInsideSuctionCircle(sheep->GetTransform().position))
            {
                isSucking = true;
                break;
            }
        }
    }

    if (!m_rescueQueue.empty())
    {
        ChangeState(CBaseState::State::RESCUE);
        CEnemyBase::Update();
        return;
    }

    // 鄒､繧梧ュ蝣ｱ繧貞叙蠕・
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_sheeps);
    // 鄒､繧後・縺ｾ縺ｨ縺ｾ繧雁愛螳夲ｼ医せ繝医Ο繝ｳ繝懊Β: 15m・・
    constexpr float flogRadiusSq = 15.0f * 15.0f;
    bool isFlockScattered = (info.maxDistance > flogRadiusSq);

    if (isSucking)
    {
        // 蜷ｸ縺・ｾｼ縺ｿ荳ｭ・壹ヵ繝ｫ迚ｧ鄒翫Δ繝ｼ繝・
        if (isFlockScattered)
        {
            // 鄒､繧後′繝舌Λ繝舌Λ 竊・COLLECTING・医・縺舌ｌ鄒翫ｒ謌ｻ縺呻ｼ・
            ChangeState(CBaseState::State::COLLECTING);
        }
        else
        {
            // 鄒､繧後′縺ｾ縺ｨ縺ｾ縺｣縺ｦ縺・ｋ 竊・DRIVING・・FO縺九ｉ驕縺悶￠繧具ｼ・
            ChangeState(CBaseState::State::DRIVING);
        }
    }
    else
    {
        // 騾壼ｸｸ譎ゑｼ壹・縺舌ｌ鄒翫′縺・◆繧画綾縺励↓陦後￥
        if (isFlockScattered)
        {
            ChangeState(CBaseState::State::COLLECTING);
        }
        else
        {
            // 鄒､繧後′縺ｾ縺ｨ縺ｾ縺｣縺ｦ縺・ｋ 竊・IDLE
            ChangeState(CBaseState::State::IDLE);
        }
    }

    CEnemyBase::Update(); // Component螳溯｡・
}


void CAShepherdDog::ChangeStateHerded(const CSheep* sheep) const
{
    for (const auto s : m_sheeps)
    {
        if (s == sheep)continue;
        s->ChangeState(CBaseState::State::HERDED);
    }
}

void CAShepherdDog::StartHerding()
{
    m_isHerding = true;
}

void CAShepherdDog::RescueSheep(CSheep* sheep)
{
    // 謨大勧蟇ｾ雎｡縺ｮ鄒翫ｒHERDED迥ｶ諷九↓螟画峩
    sheep->ChangeState(CBaseState::State::HERDED);

    // 鄒､繧悟宛蠕｡繧帝幕蟋具ｼ医∪縺髢句ｧ九＠縺ｦ縺・↑縺代ｌ縺ｰ・・
    if (!m_isHerding)
    {
        m_isHerding = true;
    }

    // 謨大勧蠕・■繝ｪ繧ｹ繝医↓霑ｽ蜉・磯㍾隍・メ繧ｧ繝・け・・
    auto it = std::find(m_rescueQueue.begin(), m_rescueQueue.end(), sheep);
    if (it == m_rescueQueue.end())
    {
        m_rescueQueue.push_back(sheep);
    }
}

void CAShepherdDog::PopRescueQueue()
{
    if (!m_rescueQueue.empty())
    {
        m_rescueQueue.erase(m_rescueQueue.begin());
    }
}

const std::vector<CSheep*>& CAShepherdDog::GetSheeps() const
{
    return m_sheeps;
}

const std::vector<CSheep*>& CAShepherdDog::GetRescueQueue() const
{
    return m_rescueQueue;
}

void CAShepherdDog::AddSheep(CSheep* sheep)
{
    m_sheeps.push_back(sheep);
}

