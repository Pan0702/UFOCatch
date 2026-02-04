#include "ShepherdDog.h"
#include "../System/Flog.h"
#include "../Component/SheperdDogComp.h"
CAShepherdDog::CAShepherdDog()
{
    m_pMesh = ObjectManager::FindGameObject<CModelRegistry>()->GetMesh("Dog");
    //動くスピード2.0f
    m_components[CBaseState::State::COLLECTING] = new CCollecting(this, 2.0f);
    //動くスピード2.0f
    m_components[CBaseState::State::DRIVING] = new CDriving(this, 2.0f);
    //スコアが２００、経験値が2.0f
    m_components[CBaseState::State::RESCUE] = new CDestroyShepherdDog(this,200,2.0f);
    m_components[CBaseState::State::RESCUE] = new CRescue(this);
    m_pState = new CBaseState(this);
    m_pComponent = m_components[CBaseState::State::IDLE];
    m_pState->Enter(CBaseState::State::IDLE);
}

CAShepherdDog::~CAShepherdDog() = default;

void CAShepherdDog::Update()
{
    // 平常時は何もしない
    if (!m_isHerding) return;

    // 救助モード：救助待ちの羊がいる場合
    if (m_isRescuing && !m_rescueQueue.empty())
    {
        // COLLECTINGが完了したら、救助完了
        CCollecting* collecting = dynamic_cast<CCollecting*>(m_components[CBaseState::State::COLLECTING]);
        if (collecting != nullptr && collecting->IsFinish())
        {
            // 救助完了：リストから削除
            m_rescueQueue.erase(m_rescueQueue.begin());

            // まだ救助待ちがいれば次の救助へ
            if (!m_rescueQueue.empty())
            {
                SetState(CBaseState::State::COLLECTING);
            }
            else
            {
                // 全員救助完了：通常モードに戻る
                m_isRescuing = false;
            }
        }
        return; // 救助モード中は通常の群れ制御をしない
    }

    // 通常モード：群れ全体を管理
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_sheeps);
    constexpr float flogRadius = 10.0f;
    if (info.maxDistance > flogRadius)
    {
        SetState(CBaseState::State::COLLECTING);
    }
    // まとまっている → Driving
    else
    {
        SetState(CBaseState::State::DRIVING);
    }
    
    CEnemyBase::Update();  // Component実行
}


void CAShepherdDog::ChangeStateHerded(const CSheep* sheep) const
{
    for (auto s : m_sheeps)
    {
        if (s == sheep)continue;
        s->SetState(CBaseState::State::HERDED);
    }
}

void CAShepherdDog::StartHerding()
{
    m_isHerding = true;
}

void CAShepherdDog::RescueSheep(CSheep* sheep)
{
    // 救助対象の羊をHERDED状態に変更
    sheep->SetState(CBaseState::State::HERDED);

    // 群れ制御を開始（まだ開始していなければ）
    if (!m_isHerding)
    {
        m_isHerding = true;
    }

    // 救助待ちリストに追加（重複チェック）
    auto it = std::find(m_rescueQueue.begin(), m_rescueQueue.end(), sheep);
    if (it == m_rescueQueue.end())
    {
        m_rescueQueue.push_back(sheep);
    }
    m_rescueQueue.push_back(sheep);
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
