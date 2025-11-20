#pragma once
#include "../04_FrameWork/SceneBase.h"

class CResultScene : public SceneBase
{
public:
    CResultScene();
    ~CResultScene();

private:
    void Draw() override;
    void Update() override;
    void CalcScore();
    void DrawScore(const int& score) const;

private:
    int m_score;
    int num;
    CSprite* m_pSprite;
    CSpriteImage* m_pResultImage;
    CSpriteImage* m_pNumImage;
    CSpriteImage* m_pRankImage;
};
