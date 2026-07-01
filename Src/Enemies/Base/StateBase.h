#pragma once
#include <queue>

/// <summary>敵キャラクター共通の移動、衝突、状態制御を持つ基底クラス</summary>
class CEnemyBase;
/// <summary>敵AIの行動部品が継承する基底クラス</summary>
class CComponentBase;

/// <summary>敵AIで使う Base State の情報と処理をまとめる型</summary>
class CBaseState
{
public:
    /// CBaseState を初期化する
    /// @param e e に渡す値
    CBaseState(CEnemyBase* e);
    /// CBaseState の終了処理を行う
    virtual ~CBaseState() = default;

    /// <summary>State で扱う状態や種別を表す列挙型</summary>
    enum class State : uint8_t
    {
        IDLE = 0,
        WALK,
        SUCTION,
        UNIQUEACTION,
        DESTROY,
        SEARCH,
        FIND_PLAYER,
        ESCAPE,
        HERDED,
        PANIC, // 3. パニック（群れがいなくなって暴走）
        COLLECTING,
        DRIVING,
        RESCUE,
    };

    /// Enter の処理を行う
    /// @param type type に渡す値
    virtual void Enter(State type);
    /// 毎フレームの状態を更新する
    virtual void Update();
    /// Exit の処理を行う
    virtual void Exit();

    /// Current State を取得する
    /// @return 処理結果
    State GetCurrentState() const { return m_kType; }

    /// Next State を設定する
    void SetNextState();

protected:
    /// Next State Pop を返す
    /// @return 処理結果
    CBaseState::State NextStatePop();

    /// Clamp Rotate Y を返す
    /// @param angle 角度
    /// @return 計算結果の値
    static float ClampRotateY(const float& angle);
    CEnemyBase* m_pEnemy;
    State m_kType;
    CComponentBase* m_pComponent;

private:
    static constexpr int NEXT_STATE_MAX_SIZE = 3;
    std::queue<CBaseState::State> actionQueue;
};
