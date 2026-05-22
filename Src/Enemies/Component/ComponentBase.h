#pragma once

/// <summary>プレイヤーUFOの移動、吸引、HP、レベル処理をまとめるクラス</summary>
class CPlayer;
/// <summary>敵キャラクター共通の移動、衝突、状態制御を持つ基底クラス</summary>
class CEnemyBase;

/// <summary>敵AIの行動部品が継承する基底クラス</summary>
class CComponentBase
{
public:
    /// CComponentBase を初期化する
    CComponentBase() = default;
    /// CComponentBase の終了処理を行う
    virtual ~CComponentBase()
    {
    }

    /// Enter の処理を行う
    virtual void Enter()
    {
    }

    /// 毎フレームの状態を更新する
    virtual void Update()
    {
    }

    /// Exit の処理を行う
    virtual void Exit()
    {
        m_isFinish = false;
    }

    /// Finish を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsFinish() const { return m_isFinish; }

protected:
    CPlayer* m_pPlayer;
    CEnemyBase* m_pOwner;
    bool m_isFinish;
};
