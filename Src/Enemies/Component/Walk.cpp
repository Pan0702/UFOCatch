#include "Walk.h"
#include "../Base/EnemyBase.h"
#include "../System/EnemyRegistr.h"

CWalk::CWalk(CEnemyBase* e,float speed)
{
    m_pOwner = e;
    m_moveAmount = speed;
}

void CWalk::Enter()
{
    
    static constexpr int   kMaxRetry     = 50;     // ランダム移動の試行回数上限（境界外に出ない組み合わせが見つかるまで最大 N 回試す）
    static constexpr float kTurnAngleDeg = 180.0f; // ランダム回転の範囲（度）。
    static constexpr float kMinMove      = 1.0f;   // ランダム移動距離の範囲（最小～最大）
    static constexpr float kMaxMove      = 3.5f;   // ランダム移動距離の範囲（最小～最大）

    m_totalPosZMoveAmount = 0.0f;
    // 現在の Transform を取得
    Transform transform = m_pOwner->GetTransform();
    m_position = transform.position;

    bool foundValidMove = false;

    // ランダムに（回転量＋移動距離）を作って、境界内に収まるまでリトライ
    for (int retry = 0; retry < kMaxRetry; ++retry)
    {
        // 回転量：[-180°, +180°] をランダムに選んでラジアンへ変換
        m_turnAmount = Randomf(-kTurnAngleDeg, kTurnAngleDeg) * DegToRad;
        // 移動距離：[1.0, 3.5] をランダムに選ぶ
        m_moveAmount = Randomf(kMinMove, kMaxMove);

        // この回転＋移動で境界外に出ないなら採用して終了
        if (BoundaryCheck(m_pOwner->GetAreaSize()))
        {
            foundValidMove = true;
            break;
        }
    }

    // 最後まで見つからなかった場合：
    // 「移動しない（0）」にして、回転だけはランダムに与える
    if (!foundValidMove)
    {
        m_moveAmount = 0.0f;
        m_turnAmount = Randomf(-kTurnAngleDeg, kTurnAngleDeg) * DegToRad;
    }
    
    m_currentRotation = transform.rotation.y;
    
    //目標回転
    m_targetRotation = m_currentRotation + m_turnAmount;
    
    //回転補間を走らせるためのフラグ
    m_rotation = true;
    Animator* animator = m_pOwner->GetAnimator();
    animator->MergePlay(AnimationType::A_IDEL);
    animator->SetPlaySpeed(1.0f);
}

bool CWalk::BoundaryCheck(const VECTOR2& areaSize) const
{
    VECTOR3 tmpPos = m_position + VECTOR3(0, 0, m_moveAmount) * XMMatrixRotationY(m_turnAmount);
    if (tmpPos.x <= areaSize.x && tmpPos.x >= -areaSize.x && tmpPos.z <= areaSize.y && tmpPos.z >= -areaSize.y)
    {
        return true;
    }
    return false;
}

void CWalk::Update()
{
    if (m_rotation)
    {
        static constexpr float ROTATION_LERP_SPEED = 10.0f;
        float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
        m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
        if (abs(m_targetRotation - m_currentRotation) < 0.01f)
        {
            m_currentRotation = m_targetRotation;
            m_rotation = false;
        }
        m_pOwner->SetRotateY(ClampRotateY(m_currentRotation));
    }
    
    float moveAmount1frame = m_moveSpeed * SceneManager::DeltaTime();
    m_pOwner->AddPosition(
        VECTOR3(0, 0, moveAmount1frame) * XMMatrixRotationY(m_currentRotation));
    m_totalPosZMoveAmount += moveAmount1frame;

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        m_isFinish = true;
    }
    m_pOwner->IsSuctionCheck();
}

float CWalk::ClampRotateY(float angle)
{    float degAngle = angle * RadToDeg;
    constexpr float HALF_ROTATION_DEG = 180.0f;
    constexpr float FULL_ROTATION_DEG = 360.0f;
    while (degAngle > HALF_ROTATION_DEG)
    {
        degAngle -= FULL_ROTATION_DEG;
    }
    while (degAngle < -HALF_ROTATION_DEG)
    {
        degAngle += FULL_ROTATION_DEG;
    }

    return degAngle * DegToRad;
}