#pragma once
#include "../../Framework/SceneBase.h"
#include "../../System/GameInstance.h"

// ゲーム終了後のリザルトを表示するシーン //
class CResultScene : public SceneBase
{
public:
    /// CResultScene を初期化する
    CResultScene();
    /// CResultScene の終了処理を行う
    ~CResultScene();

private:
    /// 描画する
    void Draw() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Rank を計算する
    void CalcRank();
    /// Rank を描画する
    void DrawRank() const;
    // リザルト数値を描画する
    // @param result 表示する数値
    // @param srcY 描画するY座標 //
    /// Result Num を描画する
    /// @param result result に渡す値
    /// @param srcY srcY に渡す値
    void DrawResultNum(int result, int srcY);

private:
    int m_score;
    int m_rankImageNum;
    int m_maxScore;
    float bar;
    std::unique_ptr<CSprite> m_pSprite;
    std::unique_ptr<CSpriteImage> m_pResultImage;
    std::unique_ptr<CSpriteImage> m_pRankImage;
    CGameInstance* m_pGI;
};
