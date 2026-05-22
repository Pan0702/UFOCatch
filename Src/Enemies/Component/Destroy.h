#pragma once
#include "ComponentBase.h"

/// <summary>敵AIで使う Destroy の情報と処理をまとめる型</summary>
class CDestroy : public CComponentBase
{
public:
    /// CDestroy を初期化する
    /// @param e e に渡す値
    /// @param score score に渡す値
    /// @param exp exp に渡す値
    CDestroy(CEnemyBase* e, int score, float exp);

    /// Enter の処理を行う
    void Enter() override;

protected:
    int m_score;
    float m_exp;
};
