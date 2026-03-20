#include "Walk.h"
#include "../Base/EnemyBase.h"
#include "../Human/Human.h"
#include "../AnimalSheep/Sheep.h"
#include "../System/Flog.h"

CWalk::CWalk(CEnemyBase* e, float speed)
{
    m_pOwner = e;
    m_moveSpeed = speed;
}

void CWalk::Enter()
{
    m_isFinish = false;
    
    // 現在の Transform を取得
    Transform trans = m_pOwner->GetTransform();
    m_position = trans.position;

    bool foundValidMove = CalcRandomMove();
    
    // 最後まで見つからなかった場合、
    // 「移動しない」にして、回転だけランダムに決める
    if (!foundValidMove)
    {
        m_turnAmount = Randomf(-TURN_ANGLE_DEG, TURN_ANGLE_DEG) * DegToRad;
    }
    VECTOR2 pos,size;
    m_pOwner->GetBounds2D(pos,size);
    m_pathFinder.SetAnimSize(size);
    const VECTOR2 start = {ToVec2XZ(m_position)};
    const VECTOR2 end = {ToVec2XZ(m_targetPos)};
    m_path = m_pathFinder.SearchRoute(start, end);
    m_pathIndex = 1;
    
    m_currentRotation = trans.rotation.y;

    //目標回転
    m_targetRotation = m_currentRotation + m_turnAmount;

    //回転処理を終わらせるためのフラグ
    m_rotation = true;
    PlayWalkAnimation();
}

/// 境界内に収まるランダムな回転量と移動距離を決定する
/// ランダムに回転量（-180°～180°）と移動距離（1.0～3.5）を決定し、境界チェックに通るまで最大50回リトライする。
/// 妥当な組み合わせが見つかった場合、m_turnAmountとm_moveAmountに設定される。
/// @return 妥当な移動パラメータが見つかった場合true、最大試行回数を超えた場合false
bool CWalk::CalcRandomMove()
{
    static constexpr int MAX_RETRY = 50;// ランダム移動の試行回数上限（境界外に出ない組み合わせが見つかるまで最大 N 回試す）
    static constexpr float MIN_MOVE = 1.0f;// ランダム移動距離の範囲（最小～最大）
    static constexpr float MAX_MOVE = 3.5f;// ランダム移動距離の範囲（最小～最大）

    // Sheep専用の範囲チェック
    CSheep* sheep = dynamic_cast<CSheep*>(m_pOwner);
    CFlog* flog = nullptr;
    if (sheep != nullptr)
    {
        flog = ObjectManager::FindGameObject<CFlog>();
    }

    // ランダムに「回転量」「移動距離」を作って、境界内に収まるまでリトライ
    for (int retry = 0; retry < MAX_RETRY; ++retry)
    {
        // 回転量[-180°, +180°] をランダムに選んでラジアンに変換
        m_turnAmount = Randomf(-TURN_ANGLE_DEG, TURN_ANGLE_DEG) * DegToRad;

        // 移動距離[1.0, 3.5] をランダムに選ぶ
        const float moveAmount = Randomf(MIN_MOVE, MAX_MOVE);
        
        m_targetPos = m_position + VECTOR3(0, 0,
                                      moveAmount) * XMMatrixRotationY(m_turnAmount);

        // Sheepの場合：Flogの範囲内かチェック
        if (flog != nullptr)
        {
            VECTOR3 toCenter = flog->GetFlockCenter() - m_targetPos;
            toCenter.y = 0;
            float distanceToCenter = sqrtf(toCenter.LengthSquare());
            if (distanceToCenter <= flog->GetFlockRadius())
            {
                return true;
            }
        }
        // それ以外のエネミーの場合、自身のAreaSizeチェック
        else if (IsInsideAreaXZ(m_targetPos, m_pOwner->GetAreaSize()))
        {
            return true;
        }
    }
    return false;
}
//別のところに書いた関数で動くためコメントアウト
// /// 回転・移動後の座標が境界内に収まるかチェック
// /// @param areaSize エリアのサイズ
// /// @return 境界内ならtrue、境界外ならfalse
// bool CWalk::BoundaryCheck(const VECTOR2&
//     areaSize) const
// {
//     VECTOR3 tmpPos = m_position + VECTOR3(0, 0,
//                                           m_moveAmount) * XMMatrixRotationY(m_turnAmount);
//     if (tmpPos.x <= areaSize.x && tmpPos.x >=
//         -areaSize.x && tmpPos.z <= areaSize.y &&
//         tmpPos.z >= -areaSize.y)
//     {
//         return true;
//     }
//     return false;
// }

void CWalk::PlayWalkAnimation() const
{
    Animator* animator = m_pOwner->GetAnimator();
    animator->MergePlay(AnimationType::A_WALK);
    animator->SetPlaySpeed(1.0f);
}


void CWalk::Update()
{
    if (m_isFinish)return;
    
    if (m_pOwner->IsHuman())
    {
        m_pOwner->IsSuctionCheck();
    }
    if (m_path.empty())
    {
        static constexpr float ROTATION_LERP_SPEED = 10.0f;
        const float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
        m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
        if (abs(m_targetRotation - m_currentRotation) < 0.01f)
        {
            m_isFinish = true;
        }
        m_pOwner->SetRotateY(ClampRotateY(m_currentRotation));
        return;
    }
    const VECTOR2 nextPoint = m_path[m_pathIndex];
    const VECTOR3 nextPos = {nextPoint.x, m_position.y, nextPoint.y};
    
    // ウェイポイントの方向を向く
    VECTOR3 dir = nextPos - m_pOwner->GetTransform().position;
    dir.y = 0;
    const float targetAngle = atan2f(dir.x, dir.z);

    // 回転
    const float current = m_pOwner->GetTransform().rotation.y;
    const float t = 10.0f * SceneManager::DeltaTime();
    float angleDiff = targetAngle - current;
    angleDiff = std::remainder(angleDiff, XM_2PI); 
    const float newAngle = current + angleDiff * t;
    m_pOwner->SetRotateY(newAngle);

    // 移動
    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) 
                      * XMMatrixRotationY(newAngle);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    m_pOwner->AddPosition(moveVec);

    //目的地の計算
    VECTOR3 toTarget = nextPos - m_pOwner->GetTransform().position;
    toTarget.y = 0;
    //距離の計算
    const float disSq = toTarget.LengthSquare();
    const float cellSize = m_pathFinder.GetCellSize();
    const float reachDistSq = cellSize * 0.5f * cellSize * 0.5f;
    if (disSq < reachDistSq)
    {
        m_pathIndex++;
        if (m_pathIndex >= m_path.size() - 1)
        {
            m_isFinish = true;
            return;
        }
        while (m_pathIndex < m_path.size() - 1)
        {
            // 現在地 → 次のウェイポイントの方向
            VECTOR2 toNext = m_path[m_pathIndex] - ToVec2XZ(m_pOwner->GetTransform().position);
            // 現在地 → さらに先のウェイポイントの方向
            VECTOR2 toAfter = m_path[m_pathIndex + 1] - ToVec2XZ(m_pOwner->GetTransform().position);

            // 2つの方向の角度差が小さければ次をスキップ
            const float angleCos = dot(normalize(toNext), normalize(toAfter));
            const float threshold = cos(30.0f * DegToRad);
            if (angleCos > threshold) m_pathIndex++;
            else break;
        }
    }

    //
    // if (m_rotation)
    // {
    //     static constexpr float ROTATION_LERP_SPEED = 10.0f;
    //     float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
    //     m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
    //     if (abs(m_targetRotation - m_currentRotation) < 0.01f)
    //     {
    //         m_currentRotation = m_targetRotation;
    //         m_rotation = false;
    //     }
    //     m_pOwner->SetRotateY(ClampRotateY(m_currentRotation));
    // }
    //
    // VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(m_currentRotation);
    // moveVec = m_pOwner->CalcSlideMove(moveVec);
    // m_pOwner->AddPosition(moveVec);
    //
    //
    // if (m_pOwner->IsHuman())
    // {
    //     m_pOwner->IsSuctionCheck();
    // }

    
}

float CWalk::ClampRotateY(float angle)
{
    return std::remainder(angle, XM_2PI);
}
