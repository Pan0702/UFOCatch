#include "ResultScene.h"

#include "../GameInstance.h"
#include "../04_FrameWork/ObjectManager.h"

CResultScene::CResultScene()
{
    m_pSprite = new CSprite;
    m_pResultImage = new CSpriteImage("data/ScoreBG.jpg");
    m_pRankImage = new CSpriteImage("data/PlayUIParts.png");
    m_maxScore = 20 * 100;
    CalcRank();

}

CResultScene::~CResultScene()
{
    if (m_pResultImage != nullptr)
    {
        SAFE_DELETE(m_pResultImage);
        m_pResultImage = nullptr;
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
void CResultScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T))
    {
        SceneManager::ChangeScene("TitleScene");
    }
}


void CResultScene::Draw()
{
    m_pSprite->Draw(m_pResultImage, 0, 0, 0, 0, 1366, 768);
    DrawScore(m_score);
}


void CResultScene::CalcRank()
{
    m_score = ObjectManager::FindGameObject<CGameInstance>()->GetScore();
    float ratio = avoidZero(static_cast<float>(m_score) / static_cast<float>(m_maxScore));
    if (ratio < 0.25f)
    {
        m_rankImageNum = 3;
    }else if (ratio < 0.5f)
    {
        m_rankImageNum = 2;
    }else if (ratio < 0.75f)
    {
        m_rankImageNum = 1;
    }else if (ratio <= 1.0f)
    {
        m_rankImageNum = 0;
    }
    
}

void CResultScene::DrawScore(const int& score) const
{
    std::string str = std::to_string(score);
    int strSize = static_cast<int>(str.size());
    const float scoreStartX = 700.0f;
    const float digitWidth  = 80.0f;
    
    for (int i = 0; i < strSize; i++)
    {
        m_pSprite->Draw(m_pRankImage,scoreStartX + digitWidth * static_cast<float>(i),200,
            0 ,0,100 * (str[i] - '0'),100);
    }
    
    m_pSprite->Draw(m_pRankImage,270,340,180 * m_rankImageNum,
        230,179,308);
}
