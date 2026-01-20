#pragma once
#include "../Framework/SceneBase.h"
#include "../System/GameInstance.h"

// ゲーム終了後のリザルトを表示するシーン //
class CResultScene : public SceneBase
{
public:
    CResultScene();
    ~CResultScene();

private:
    void Draw() override;
    void Update() override;
    void CalcRank();
    void DrawRank() const;
    // リザルト数値を描画する
    // @param result 表示する数値
    // @param srcY 描画するY座標 //
    void DrawResultNum(int result, int srcY) ;

private:
    int m_score;
    int m_rankImageNum;
    int m_maxScore;
    float bar;
    CSprite* m_pSprite;
    CSpriteImage* m_pResultImage;
    CSpriteImage* m_pRankImage;
    CGameInstance* m_pGI;
};
