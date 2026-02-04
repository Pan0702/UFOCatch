#include "ShepherdDog.h"

#include "../Component/Idle.h"
#include "../System/Flog.h"
#include "../Component/SheperdDogComp.h"
#include "../System/EnemyManager.h"
#include "../Component/Walk.h"

CAShepherdDog::CAShepherdDog()
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);
    m_pGround = ObjectManager::FindGameObject<CGround>(); 
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    //動くスピード2.0f
    m_components[CBaseState::State::COLLECTING] = new CCollecting(this, 2.0f);
    //動くスピード2.0f
    m_components[CBaseState::State::DRIVING] = new CDriving(this, 2.0f);
    m_components[CBaseState::State::RESCUE] = new CRescue(this);
    m_components[CBaseState::State::IDLE] = new CIdle(this, 570.0f);
    m_components[CBaseState::State::WALK] = new CWalk(this, 1.2f);
    //スコアが２００、経験値が2.0f
    m_components[CBaseState::State::DESTROY] = new CDestroyShepherdDog(this,200,2.0f);
    m_pState = new CBaseState(this);
    m_pComponent = m_components[CBaseState::State::IDLE];
    m_pState->Enter(CBaseState::State::IDLE);
    m_pBBox = CreateBBox();
}

CAShepherdDog::~CAShepherdDog() = default;

void CAShepherdDog::Update()
{
    // UFOが吸い込み中かどうかをリアルタイムで確認
    bool isSucking = (m_pPlayer != nullptr && m_pPlayer->GetIsSuckUp());

    ImGui::Begin("ShepherdDog Debug");
    ImGui::Text("Pos: %.2f, %.2f", transform.position.x, transform.position.z);
    ImGui::Text("UFO Sucking: %s", isSucking ? "true" : "false");
    ImGui::Text("m_sheeps.size: %zu", m_sheeps.size());
    ImGui::End();

    // 群れ情報を取得
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_sheeps);
    // 群れのまとまり判定（ストロンボム: 15m）
    constexpr float flogRadiusSq = 15.0f * 15.0f;
    bool isFlockScattered = (info.maxDistance > flogRadiusSq);

    if (isSucking)
    {
        // 吸い込み中：フル牧羊モード
        if (isFlockScattered)
        {
            // 群れがバラバラ → COLLECTING（はぐれ羊を戻す）
            SetState(CBaseState::State::COLLECTING);
        }
        else
        {
            // 群れがまとまっている → DRIVING（UFOから遠ざける）
            SetState(CBaseState::State::DRIVING);
        }
    }
    else
    {
        // 通常時：はぐれ羊がいたら戻しに行く
        if (isFlockScattered)
        {
            SetState(CBaseState::State::COLLECTING);
        }
        else
        {
            // 群れがまとまっている → IDLE
            SetState(CBaseState::State::IDLE);
        }
    }

    CEnemyBase::Update();  // Component実行
}


void CAShepherdDog::ChangeStateHerded(const CSheep* sheep) const
{
    for (const auto s : m_sheeps)
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
