#pragma once
#include "ComponentBase.h"

/// <summary>敵AIで使う Idle の情報と処理をまとめる型</summary>
class CIdle : public CComponentBase
{
public:
    /// CIdle を初期化する
    /// @param e e に渡す値
    /// @param endFrame endFrame に渡す値
    CIdle(CEnemyBase* e, float endFrame);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Idle Stop の処理を行う
    void IdleStop();
    /// Idle Anim の処理を行う
    void IdleAnim();
    /// Animation Finish を返す
    /// @return 成功または条件を満たす場合 true
    bool AnimationFinish() const;

    enum
    {
        ANIMATION,
        STOP,
    };

    int m_idleState = 0;
    float m_timerCnt = 0;
    bool m_isHuman = false;
    const float m_endFrame;
};
