#pragma once
#include "../Framework/SceneBase.h"
#include "../System/GameInstance.h"

// 繧ｲ繝ｼ繝邨ゆｺ・ｾ後・繝ｪ繧ｶ繝ｫ繝医ｒ陦ｨ遉ｺ縺吶ｋ繧ｷ繝ｼ繝ｳ //
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
    // 繝ｪ繧ｶ繝ｫ繝域焚蛟､繧呈緒逕ｻ縺吶ｋ
    // @param result 陦ｨ遉ｺ縺吶ｋ謨ｰ蛟､
    // @param srcY 謠冗判縺吶ｋY蠎ｧ讓・//
    void DrawResultNum(int result, int srcY) ;

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
