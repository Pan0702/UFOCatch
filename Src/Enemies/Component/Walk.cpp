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
    
    m_totalPosZMoveAmount = 0.0f;
    // 現在の Transform を取得
    Transform transform = m_pOwner->GetTransform();
    m_position = transform.position;

    bool foundValidMove = CalcRandomMove();
    
    // 最後まで見つからなかった場合：
    // 「移動しない（0）」にして、回転だけはランダムに与える
    if (!foundValidMove)
    {
        m_turnAmount = Randomf(-kTurnAngleDeg, kTurnAngleDeg) * DegToRad;
    }

    m_currentRotation = transform.rotation.y;

    //目標回転
    m_targetRotation = m_currentRotation + m_turnAmount;

    //回転補間を走らせるためのフラグ
    m_rotation = true;
    PlayWalkAnimation();
}

/// 境界内に収まるランダムな回転量と移動距離を 生成する
/// ランダムに回転量（-180°～+180°）と移動距離  （1 .0～3.5）を生成し、 境界チェックに通るまで最大50回リトライする。
/// 有効な組み合わせが見つかった場合、m_turnAm  ountとm_moveAmountに設定される。
/// @return 有効な移動パラメータが見つかった場  合true、 最大試行回数を超えた場合false
bool CWalk::CalcRandomMove()
{
    static constexpr int kMaxRetry = 50;// ランダム移動の試行回数上限（境界外に出ない組み合わせが見つかるまで最大 N 回試す）
    static constexpr float kMinMove = 1.0f;// ランダム移動距離の範囲（最小～最大）
    static constexpr float kMaxMove = 3.5f;// ランダム移動距離の範囲（最小～最大）

    // Sheep専用の範囲チェック
    CSheep* sheep = dynamic_cast<CSheep*>(m_pOwner);
    CFlog* flog = nullptr;
    if (sheep != nullptr)
    {
        flog = ObjectManager::FindGameObject<CFlog>();
    }

    // ランダムに（回転量＋移動距離）を作って、境界内に収まるまでリトライ
    for (int retry = 0; retry < kMaxRetry; ++retry)
    {
        // 回転量：[-180°, +180°] をランダムに選んでラジアンへ変換
        m_turnAmount = Randomf(-kTurnAngleDeg, kTurnAngleDeg) * DegToRad;

        // 移動距離：[1.0, 3.5] をランダムに選ぶ
        m_moveAmount = Randomf(kMinMove, kMaxMove);

        VECTOR3 tmpPos = m_position + VECTOR3(0, 0,
                                      m_moveAmount) * XMMatrixRotationY(m_turnAmount);

        // Sheepの場合、Flogの範囲内かチェック
        if (flog != nullptr)
        {
            VECTOR3 toCenter = flog->GetFlockCenter() - tmpPos;
            toCenter.y = 0;
            float distanceToCenter = sqrtf(toCenter.LengthSquare());
            if (distanceToCenter <= flog->GetFlockRadius())
            {
                return true;
            }
        }
        // 他のエネミーの場合、既存のAreaSizeチェック
        else if (IsInsideAreaXZ(tmpPos, m_pOwner->GetAreaSize()))
        {
            return true;
        }
    }
    return false;
}
//別のところに書いた関数で動くか試すためコメントアウト
// /// 回転・移動後の位置が境界内に収まるかチェック
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

void CWalk::PlayWalkAnimation()
{
    Animator* animator = m_pOwner->GetAnimator();
    animator->MergePlay(AnimationType::A_WALK);
    animator->SetPlaySpeed(1.0f);
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

    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(m_currentRotation);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    m_pOwner->AddPosition(moveVec);
    m_totalPosZMoveAmount += m_moveSpeed * SceneManager::DeltaTime();

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        m_isFinish = true;
    }
    if (m_pOwner->IsHuman())
    {
        m_pOwner->IsSuctionCheck();
    }
}

float CWalk::ClampRotateY(float angle)
{
    return std::remainder(angle, XM_2PI);
}
