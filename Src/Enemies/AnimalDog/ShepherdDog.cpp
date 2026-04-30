#include "ShepherdDog.h"

#include "../Component/ComponentFwd.h"
#include "../System/Flock.h"
#include "../Component/ShepherdDogComp.h"

CAShepherdDog::CAShepherdDog()
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    //移動スピード:2.0f
    m_components[CBaseState::State::COLLECTING] = std::make_unique<CCollecting>(this, 8.0f);
    //移動スピード:2.0f
    m_components[CBaseState::State::DRIVING] = std::make_unique<CDriving>(this, 8.0f);
    m_components[CBaseState::State::RESCUE] = std::make_unique<CRescue>(this);
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this, 570.0f);
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this, 8.0f);
    //スコア：200, 経験値：2.0f
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroyShepherdDog>(this, 200, 2.0f);
    m_pState = std::make_unique<CBaseState>(this);
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState->Enter(CBaseState::State::IDLE);
    m_pBBox = CreateBBox();
}

CAShepherdDog::CAShepherdDog(const VECTOR3& iniPos)
{
    transform.position = iniPos;
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();

    constexpr float ALGORITHM_MOVE_SPEED = 3.0f;
    constexpr float MOVE_SPEED = 1.2f;
    m_components[CBaseState::State::COLLECTING] = std::make_unique<CCollecting>(this, ALGORITHM_MOVE_SPEED);
    m_components[CBaseState::State::DRIVING] = std::make_unique<CDriving>(this, ALGORITHM_MOVE_SPEED);
    m_components[CBaseState::State::RESCUE] = std::make_unique<CRescue>(this);
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this, 570.0f);
    m_components[CBaseState::State::WALK] = std::make_unique<CShepherdDogWalk>(this, MOVE_SPEED);
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
    // DESTROY 中は状態判定を上書きしない
    if (GetCurrentState() == CBaseState::State::DESTROY)
    {
        CEnemyBase::Update();
        return;
    }

    RemoveDeadSheep();

    //救出待ちの羊がいる場合は、通常の群れ制御より救出を優先する//
    if (!m_rescueQueue.empty())
    {
        ChangeState(CBaseState::State::RESCUE);
        CEnemyBase::Update();
        return;
    }

    const bool isSucking = IsAnySheepBeingSucked();
    const bool isScattered = IsFlockScattered();

    if (isSucking)
    {
        // 吸い込み中：散ってれば回収、まとまってればUFOから遠ざける
        ChangeState(isScattered ? CBaseState::State::COLLECTING : CBaseState::State::DRIVING);
    }
    else
    {
        // 通常時：散ってれば回収、まとまってれば徘徊
        ChangeState(isScattered ? CBaseState::State::COLLECTING : CBaseState::State::WALK);
    }

    CEnemyBase::Update();
}

void CAShepherdDog::RemoveDeadSheep()
{
    auto isDead = [](CSheep* s) { return !ObjectManager::IsExist(s); };
    std::erase_if(m_sheeps, isDead);
    std::erase_if(m_rescueQueue, isDead);
}

bool CAShepherdDog::IsAnySheepBeingSucked() const
{
    if (m_pPlayer == nullptr || !m_pPlayer->GetIsSuckUp()) return false;

    //登録されている羊の中に吸い込み円内の個体がいるか確認する//
    for (const auto sheep : m_sheeps)
    {
        if (sheep != nullptr && m_pPlayer->IsInsideSuctionCircle(sheep->GetTransform().position))
        {
            return true;
        }
    }
    return false;
}

bool CAShepherdDog::IsFlockScattered() const
{
    if (m_pFlock == nullptr) return false;

    //1匹でも群れ円の外にいれば、回収が必要な散らばり状態とする//
    for (CSheep* sheep : m_sheeps)
    {
        if (!m_pFlock->ContainPos(sheep->GetTransform().position))
        {
            return true;
        }
    }
    return false;
}


void CAShepherdDog::ChangeStateHerded(const CSheep* sheep) const
{
    //指定された羊以外をHERDED状態に変更して群れ行動へ戻す//
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
