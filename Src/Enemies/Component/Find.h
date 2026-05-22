#pragma once
#include "ComponentBase.h"
#include "../Human/Human.h"

/// <summary>敵AIで使う Find の情報と処理をまとめる型</summary>
class CFind : public CComponentBase
{
public:
    /// CFind を初期化する
    /// @param human human に渡す値
    CFind(CHuman* human);
    /// CFind の終了処理を行う
    ~CFind() = default;

    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    CHuman* m_pOwner;
};
