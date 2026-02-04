#include "SheperdDogComp.h"
#include "../AnimalDog//ShepherdDog.h"  // パスは調整してください
#include "../System/Flog.h"

CCollecting::CCollecting(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CCollecting::Enter()
{
    m_isFinish = false;

    // 群れ情報を取得
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());

    // はぐれ羊がいない場合は終了
    if (info.furthestSheep == nullptr)
    {
        m_isFinish = true;
        return;
    }

    // はぐれ羊の位置
    const VECTOR3 sheepPos = info.furthestSheep->GetTransform().position;
    // はぐれ羊から重心への方向
    VECTOR3 toCentroid = info.centroid - sheepPos;
    toCentroid.y = 0;

    // ゼロベクトル対策
    if (toCentroid.LengthSquare() < 0.0001f)
    {
        m_isFinish = true;
        return;
    }

    normalize(toCentroid);
    // 羊の背後に立つ距離（ストロンボム: 5m）
    constexpr float behindDistance = 5.0f;
    // 背後に回り込む位置を計算
    m_targetPos = sheepPos - toCentroid * behindDistance;
}

void CCollecting::Update()
{
    // 目標位置へ移動
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    direction.y = 0;
    const float distanceSq = direction.LengthSquare();
    constexpr float arrivalThresholdSq = 0.25f;
    // 目標に到達したら終了
    if (distanceSq < arrivalThresholdSq)
    {
        m_isFinish = true;
        return;
    }

    normalize(direction);

    // 移動方向に回転
    float targetAngle = atan2f(direction.x, direction.z);
    m_pOwner->SetRotateY(targetAngle);

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

    // デバッグ：羊の数を確認
    size_t sheepCount = m_pOwner->GetSheeps().size();

    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());
    CPlayer* player = ObjectManager::FindGameObject<CPlayer>();
    if (player == nullptr) return;

    VECTOR3 ufoPos = player->GetTransform().position;
    VECTOR3 centroid = info.centroid;

    // デバッグ出力
    ImGui::Begin("CDriving Enter Debug");
    ImGui::Text("SheepCount: %zu", sheepCount);
    ImGui::Text("Centroid: %.2f, %.2f, %.2f", centroid.x, centroid.y, centroid.z);
    ImGui::Text("UFO Pos: %.2f, %.2f, %.2f", ufoPos.x, ufoPos.y, ufoPos.z);
    ImGui::End();

    // UFOから群れへの方向（逃げる方向）
    VECTOR3 escapeDir = centroid - ufoPos;
    escapeDir.y = 0;

    // ゼロベクトル対策：UFOと群れが同じ位置ならデフォルト方向
    float lengthSq = escapeDir.LengthSquare();
    if (lengthSq < 0.0001f)
    {
        escapeDir = VECTOR3(0, 0, 1);  // デフォルト：Z+方向
    }
    else
    {
        escapeDir = escapeDir / sqrtf(lengthSq);  // 正規化
    }

    // 群れの後ろ（UFOから見て群れの向こう側）に立つ
    float pushDistance = 5.0f;
    m_targetPos = centroid + escapeDir * pushDistance;
}

void CDriving::Update()
{
    // 目標位置へ移動
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    direction.y = 0;
    const float distanceSq = direction.LengthSquare();

    // デバッグ出力
    ImGui::Begin("Dog Driving");
    ImGui::Text("TargetPos: %.2f, %.2f, %.2f", m_targetPos.x, m_targetPos.y, m_targetPos.z);
    ImGui::Text("CurrentPos: %.2f, %.2f, %.2f", currentPos.x, currentPos.y, currentPos.z);
    ImGui::Text("DistanceSq: %.2f", distanceSq);
    ImGui::End();

    constexpr float arrivalThresholdSq = 0.25f;
    // 目標に到達したら終了
    if (distanceSq < arrivalThresholdSq)
    {
        m_isFinish = true;
        return;
    }

    normalize(direction);

    // 移動方向に回転
    float targetAngle = atan2f(direction.x, direction.z);
    m_pOwner->SetRotateY(targetAngle);

    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}

CRescue::CRescue(CAShepherdDog* dog)
{
    m_pOwner = dog;
    m_phase = Phase::APPROACH_SHEEP;
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

    const VECTOR3 myPos = m_pOwner->GetTransform().position;
    const VECTOR3 sheepPos = m_targetSheep->GetTransform().position;
    constexpr float m_moveSpeed = 2.0f;

    switch (m_phase)
    {
    case Phase::APPROACH_SHEEP:
        {
            // 羊への方向
            const VECTOR3 toSheep = sheepPos - myPos;
            const float distance = toSheep.LengthSquare();
            constexpr float m_arrivalDistance = 1.0f; // 重心到達判定距離

            // 羊の近くに到達したらフェーズ2へ
            if (distance < Pow2(m_arrivalDistance))
            {
                m_phase = Phase::GUIDE_TO_CENTER;
                break;
            }

            // 羊に近づく
            normalize(toSheep);
            // 移動方向に回転
            float targetAngle = atan2f(toSheep.x, toSheep.z);
            m_pOwner->SetRotateY(targetAngle);
            m_pOwner->AddPosition(toSheep * m_moveSpeed * SceneManager::DeltaTime());
        }
        break;

    case Phase::GUIDE_TO_CENTER:
        {
            // 羊から重心への方向
            VECTOR3 toCentroid = m_centroid - sheepPos;
            toCentroid.y = 0;
            const float distance = toCentroid.LengthSquare();

            // 重心に到達したら完了
            constexpr float m_approachDistance = 2.0f; // 羊に近づく距離`
            if (distance < Pow2(m_approachDistance))
            {
                m_pOwner->PopRescueQueue(); // キューから削除
                m_isFinish = true;
                break;
            }

            // 羊の背後から重心方向へプレッシャーをかける位置
            normalize(toCentroid);
            static constexpr float m_behindDistance = 2.0f; // 羊の背後に立つ距離
            const VECTOR3 behindPos = sheepPos - toCentroid * m_behindDistance;

            // その位置へ移動
            VECTOR3 toBehind = behindPos - myPos;
            toBehind.y = 0;
            normalize(toBehind);
            // 移動方向に回転
            float targetAngle = atan2f(toBehind.x, toBehind.z);
            m_pOwner->SetRotateY(targetAngle);
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
