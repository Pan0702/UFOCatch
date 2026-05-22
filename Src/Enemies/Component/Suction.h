#pragma once
#include "ComponentBase.h"

/// <summary>敵AIで使う Suction の情報と処理をまとめる型</summary>
class CSuction : public CComponentBase
{
public:
    /// CSuction を初期化する
    /// @param e e に渡す値
    CSuction(CEnemyBase* e);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

    /// Finish Suction を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsFinishSuction() const { return m_isFinishSuction; }

private:
    bool m_isFinishSuction;
};
