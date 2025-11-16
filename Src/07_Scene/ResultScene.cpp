#include "ResultScene.h"

#include "../GameInstance.h"
#include "../04_FrameWork/ObjectManager.h"

CResultScene::CResultScene()
{
    m_pSprite = new CSprite;
    m_pResultImage = new CSpriteImage("data/Result.png");
    m_pNumImage = new CSpriteImage("data/Vector.png");
    m_pRankImage = new CSpriteImage("data/Rank.png");
    CalcScore();
    num = 0;
}

CResultScene::~CResultScene()
{
    if (m_pResultImage != nullptr)
    {
        SAFE_DELETE(m_pResultImage);
        m_pResultImage = nullptr;
    }
    if (m_pNumImage != nullptr)
    {
        SAFE_DELETE(m_pNumImage);
        m_pNumImage = nullptr;
    }
    if (m_pRankImage != nullptr)
    {
        SAFE_DELETE(m_pRankImage);
        m_pRankImage = nullptr;
    }
    if (m_pSprite != nullptr)
    {
        SAFE_DELETE(m_pSprite);
        m_pSprite = nullptr;
    }
}

void CResultScene::Draw()
{
    ImGui::Begin("Result");
    ImGui::SliderInt("Score", &m_score, 0, 1000);
    ImGui::End();
    m_pSprite->Draw(m_pResultImage, 0, 0, 0, 0, 1366, 768);
    DrawScore(m_score);
}

void CResultScene::CalcScore()
{
    m_score = ObjectManager::FindGameObject<CGameInstance>()->GetScore();
}

void CResultScene::DrawScore(const int& score) const
{
    std::string str = std::to_string(score);
    int strSize = static_cast<int>(str.size());
    const float scoreStartX = 700.0f;
    const float digitWidth  = 80.0f;
    
    float scoreCenterX = scoreStartX + (strSize * digitWidth) / 2.0f;
    
    for (int i = 0; i < strSize; i++)
    {
        m_pSprite->Draw(
            m_pNumImage,
            scoreStartX + digitWidth * i,
            260,
            45 * (str[i] - '0'),
            0,
            45,
            50,
            80,
            80
        );
    }
    
    constexpr float rankWidth = 182.0f;
    float rankDrawX = (scoreCenterX - rankWidth / 2.0f) + 20;

    m_pSprite->Draw(
        m_pRankImage,
        rankDrawX,
        470,
        182 * num,
        0,
        182,
        183
    );
}
