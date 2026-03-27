#include "ShepherdDog.h"

#include "../Component/ComponentFwd.h"
#include "../System/Flog.h"
#include "../Component/SheperdDogComp.h"

CAShepherdDog::CAShepherdDog()
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    //移動スピード:2.0f
    m_components[CBaseState::State::COLLECTING] = std::make_unique<CCollecting>(this, 2.0f);
    //移動スピード:2.0f
    m_components[CBaseState::State::DRIVING] = std::make_unique<CDriving>(this, 2.0f);
    m_components[CBaseState::State::RESCUE] = std::make_unique<CRescue>(this);
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this, 570.0f);
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this, 1.2f);
    //スコア：200, 経験値：2.0f
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroyShepherdDog>(this, 200, 2.0f);
    m_pState = std::make_unique<CBaseState>(this);
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState->Enter(CBaseState::State::IDLE);
    m_pBBox = CreateBBox();
}

CAShepherdDog::~CAShepherdDog() = default;

void CAShepherdDog::Update()
{
    // UFOが羊を吸い込み中かどうかをリアルタイムで判定
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

    // 群れの状況を把握
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_sheeps);
    // 群れがまとまり判定（ストロングボム: 15m）
    constexpr float flogRadiusSq = 15.0f * 15.0f;
    bool isFlockScattered = (info.maxDistance > flogRadiusSq);

    if (isSucking)
    {
        // 羊吸い込み中→レスキューモード
        if (isFlockScattered)
        {
            // 群れがバラバラ →「COLLECTING」（一番遠い羊を戻す）
            ChangeState(CBaseState::State::COLLECTING);
        }
        else
        {
            // 群れがまとまっている →「DRIVING」（UFOから遠ざける）
            ChangeState(CBaseState::State::DRIVING);
        }
    }
    else
    {
        // 通常時→一番遠い羊がいれば戻しに行く
        if (isFlockScattered)
        {
            ChangeState(CBaseState::State::COLLECTING);
        }
        else
        {
            // 群れがまとまっている → IDLE
            ChangeState(CBaseState::State::IDLE);
        }
    }

    CEnemyBase::Update(); // Component更新
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
    // 救出対象の羊をHERDED状態に設定
    sheep->ChangeState(CBaseState::State::HERDED);

    // 群れ誘導を開始（まだ開始していなければ）
    if (!m_isHerding)
    {
        m_isHerding = true;
    }

    // 救出待ちリストに追加（重複チェック）
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
