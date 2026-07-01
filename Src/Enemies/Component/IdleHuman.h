#pragma once
#include "ComponentBase.h"
#include "../Human/Human.h"

/// <summary>敵AIで使う Idle Human の情報と処理をまとめる型</summary>
class CIdleHuman : public CComponentBase
{
public:
    /// CIdleHuman を初期化する
    /// @param e e に渡す値
    CIdleHuman(CHuman* e);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

    /// Exit の処理を行う
    void Exit() override;

private:
    /// Idle Anim の処理を行う
    void IdleAnim();
    /// Idle Search の処理を行う
    void IdleSearch();
    /// Look Around Anim の処理を行う
    void LookAroundAnim();
    /// Current Frame を取得する
    /// @return 計算結果の値
    float GetCurrentFrame() const;
    /// Animation Finish を返す
    /// @return 成功または条件を満たす場合 true
    bool AnimationFinish() const;

    enum
    {
        ANIMATION,
        SEARCH,
    };

    int m_idleState = 0;
    float m_timerCnt = 0;
    bool m_isHuman = false;

    int frameCnt = 0;
    float currentAngle = 0;
    float animationTime = 0;

    // CIdleHuman専用のオーナー（CHuman型） //
    CHuman* m_pOwner;
};
