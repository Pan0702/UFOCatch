#include "SheperDog.h"
#include "../AnimalDog//ShepherdDog.h"  // パスは調整してください
#include "../System/Flog.h"

CCollectiong::CCollectiong(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CCollectiong::Enter()
{
    m_isFinish = false;

    // 群れ情報を取得
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());

    // はぐれ羊の位置
    VECTOR3 sheepPos = info.furthestSheep->GetTransform().position;

    // はぐれ羊から重心への方向
    VECTOR3 toCentroid = info.centroid - sheepPos;
    normalize(toCentroid);

    static constexpr float m_behindDistance = 3.0f;
    // 背後に回り込む位置を計算
    m_targetPos = sheepPos - toCentroid * m_behindDistance;
}

void CCollectiong::Update()
{
    // 目標位置へ移動
    VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    float distance = direction.LengthSquare();

    // 目標に到達したら終了
    if (distance < 0.5f)
    {
        m_isFinish = true;
        return;
    }

    normalize(direction);
    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}

CDriving::CDriving(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CDriving::Enter()
{
    m_isFinish = false;

    // 群れ情報を取得
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());

    // UFO（Player）の位置を取得
    VECTOR3 ufoPos = ObjectManager::FindGameObject<CPlayer>()->GetTransform().position;
    VECTOR3 centroid = info.centroid;

    // UFOから群れへの方向（逃げる方向）
    VECTOR3 escapeDir = centroid - ufoPos;
    normalize(escapeDir);

    // 群れとUFOの間に割り込む位置
    m_targetPos = centroid - escapeDir * m_interceptDistance;
}

void CDriving::Update()
{
    // 目標位置へ移動
    VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    float distance = direction.LengthSquare();

    // 目標に到達したら終了
    if (distance < 0.5f)
    {
        m_isFinish = true;
        return;
    }

    normalize(direction);
    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}

CRescue::CRescue(CAShepherdDog* dog)
{
    m_pOwner = dog;
}

void CRescue::Enter()
{
    m_isFinish = false;

    // 救助キューから対象を取得
    if (m_pOwner->GetRescueQueue().empty())
    {
        m_isFinish = true;
        return;
    }

    m_targetSheep = m_pOwner->GetRescueQueue().front();

    // 群れの重心を計算
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());
    m_centroid = info.centroid;

    // フェーズ1: 羊に近づくところから開始
    m_phase = Phase::APPROACH_SHEEP;
}

void CRescue::Update()
{
    if (m_targetSheep == nullptr)
    {
        m_isFinish = true;
        return;
    }

    VECTOR3 myPos = m_pOwner->GetTransform().position;
    VECTOR3 sheepPos = m_targetSheep->GetTransform().position;

    switch (m_phase)
    {
    case Phase::APPROACH_SHEEP:
        {
            // 羊への方向
            VECTOR3 toSheep = sheepPos - myPos;
            float distance = toSheep.LengthSquare();

            // 羊の近くに到達したらフェーズ2へ
            if (distance < m_approachDistance * m_approachDistance)
            {
                m_phase = Phase::GUIDE_TO_CENTER;
                break;
            }

            // 羊に近づく
            normalize(toSheep);
            m_pOwner->AddPosition(toSheep * m_moveSpeed * SceneManager::DeltaTime());
        }
        break;

    case Phase::GUIDE_TO_CENTER:
        {
            // 羊から重心への方向
            VECTOR3 toCentroid = m_centroid - sheepPos;
            float distance = toCentroid.LengthSquare();

            // 重心に到達したら完了
            if (distance < m_arrivalDistance * m_arrivalDistance)
            {
                m_pOwner->PopRescueQueue(); // キューから削除
                m_isFinish = true;
                break;
            }

            // 羊の背後から重心方向へプレッシャーをかける位置
            normalize(toCentroid);
            VECTOR3 behindPos = sheepPos - toCentroid * 2.0f;

            // その位置へ移動
            VECTOR3 toBehind = behindPos - myPos;
            normalize(toBehind);
            m_pOwner->AddPosition(toBehind * m_moveSpeed * SceneManager::DeltaTime());
        }
        break;
    }
}


CDestroyShepherdDog::CDestroyShepherdDog(CAShepherdDog* dog, int score, float exp)
: CDestroy(dog, score, exp)
{
    m_pDog = dog;
}

void CDestroyShepherdDog::Enter()
{
    // 担当羊をPANIC化
    for (auto sheep : m_pDog->GetSheeps())
    {
        if (sheep != nullptr)
        {
            sheep->SetState(CBaseState::State::PANIC);
        }
    }

    // 元のDestroy処理
    CDestroy::Enter();
}
