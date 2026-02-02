#include "ShepherdDog.h"

#include "../Component/SheperDog.h"

CAShepherdDog::CAShepherdDog()
{
    
    m_components[CBaseState::State::COLLECTING] = new CCollectiong(this,2.0f);
    m_components[CBaseState::State::DRIVING] = new CDriving(this,2.0f);
}

void CAShepherdDog::Update()
{
    if (!m_isHerding) return;

    // 救助モード：救助待ちの羊がいる場合
    if (m_isRescuing && !m_rescueQueue.empty())
    {
        // COLLECTINGが完了したら、救助完了
        CCollectiong* collecting = dynamic_cast<CCollectiong*>(m_components[CBaseState::State::COLLECTING]);
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
        return;  // 救助モード中は通常の群れ制御をしない
    }

    // 通常モード：群れ全体を管理
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_sheeps);
    if (info.maxDistance > 10.0f)
    {
        SetState(CBaseState::State::COLLECTING);
    }
    else
    {
        SetState(CBaseState::State::DRIVING);
    }
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
    if (!m_isHerding) {
        m_isHerding = true;
    }

    // 救助待ちリストに追加（重複チェック）
    auto it = std::find(m_rescueQueue.begin(), m_rescueQueue.end(), sheep);
    if (it == m_rescueQueue.end()) {
        m_rescueQueue.push_back(sheep);
    }

    // 現在救助中でなければ、すぐに救助モードに移行
    if (!m_isRescuing) {
        m_isRescuing = true;
        SetState(CBaseState::State::COLLECTING);
    }
}
