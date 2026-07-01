#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/TextRenderer.h"

/// <summary>シーンで使う Result UI の情報と処理をまとめる型</summary>
class CResultUI : public CUIBase
{
public:
    /// CResultUI を初期化する
    CResultUI();

private:
    /// Back を初期化する
    void InitBack();
    /// Rank を初期化する
    void InitRank();
    /// Result Nums を初期化する
    void InitResultNums();
    /// Digits を初期化する
    /// @param value value に渡す値
    /// @param srcY srcY に渡す値
    void InitDigits(int value, float srcY);
    /// Rank を計算する
    /// @return 処理結果の数値
    int CalcRank();

    CSpriteImage* m_pSprite = nullptr;
    CUIText* m_pRank = nullptr;
};
