#include "ResultScene.h"

#include "../System/GameInstance.h"
#include "../Framework/ObjectManager.h"

CResultScene::CResultScene()
{
    
    m_pSprite = std::make_unique<CSprite>(); 
    m_pResultImage = std::make_unique<CSpriteImage>("data/ScoreBG.jpg");
    m_pRankImage = std::make_unique<CSpriteImage>("data/PlayUIParts.png");
    m_pGI = ObjectManager::FindGameObject<CGameInstance>();
}

CResultScene::~CResultScene()
{

}
void CResultScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeScene("TitleScene");
    }
   // CalcRank();
}


void CResultScene::Draw()
{
    m_pSprite->Draw(m_pResultImage.get(), 0, 0, 0, 0, 1366, 768);  // 逕ｻ髱｢繧ｵ繧､繧ｺ蜈ｨ菴薙↓閭梧勹繧呈緒逕ｻ //
    CalcRank();
    DrawRank();
    DrawResultNum(m_pGI->GetDiscovery(),60);    // 逋ｺ隕区焚縺ｮY蠎ｧ讓・//
    DrawResultNum(m_pGI->GetSaw(),217);         // 逶ｮ謦・焚縺ｮY蠎ｧ讓・//
    DrawResultNum(m_pGI->GetCapture(),387);     // 謐慕佐謨ｰ縺ｮY蠎ｧ讓・//
}

////////////////////
// 繧ｹ繧ｳ繧｢縺ｫ蝓ｺ縺･縺・※繝ｩ繝ｳ繧ｯ繧定ｨ育ｮ励☆繧・//
////////////////////
void CResultScene::CalcRank()
{
    m_score = m_pGI->GetScore()  - (m_pGI->GetDiscovery() * 3) - (m_pGI->GetSaw());  // 繝壹リ繝ｫ繝・ぅ繧貞ｷｮ縺怜ｼ輔＞縺滓怙邨ゅせ繧ｳ繧｢ //
    float ratio = avoidZero(static_cast<float>(m_score) / static_cast<float>(m_pGI->GetMaxScore()));
    if (ratio < 0.30f)  // 30%譛ｪ貅縺ｧ繝ｩ繝ｳ繧ｯD //
    {
        m_rankImageNum = 3;
    }else if (ratio < 0.60f)  // 30%譛ｪ貅縺ｧ繝ｩ繝ｳ繧ｯC //
    {
        m_rankImageNum = 2;
    }else if (ratio < 0.90f)  // 90%譛ｪ貅縺ｧ繝ｩ繝ｳ繧ｯB //
    {
        m_rankImageNum = 1;
    }else if (ratio <= 1.0f)  // 90%莉･荳翫〒繝ｩ繝ｳ繧ｯA //
    {
        m_rankImageNum = 0;
    }

}

////////////////////
// 繝ｩ繝ｳ繧ｯ逕ｻ蜒上ｒ謠冗判縺吶ｋ //
////////////////////
void CResultScene::DrawRank() const
{
    m_pSprite->Draw(m_pRankImage.get(),270,340,180 * m_rankImageNum,  // 繝ｩ繝ｳ繧ｯ縺斐→縺ｫ讓ｪ縺ｫ縺壹ｉ縺励◆逕ｻ蜒上ｒ謠冗判 //
        230,179,308);
}

////////////////////
// 繝ｪ繧ｶ繝ｫ繝域焚蛟､繧呈緒逕ｻ縺吶ｋ
// @param result 陦ｨ遉ｺ縺吶ｋ謨ｰ蛟､
// @param srcY 謠冗判縺吶ｋY蠎ｧ讓・//
////////////////////
void CResultScene::DrawResultNum(int result,int srcY)
{
    // 譯∵焚繧定ｨ育ｮ・//
    int count = 0;
    int tmp = result;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    if (count == 0) count = 1;  // 0縺ｮ蝣ｴ蜷医・1譯√→縺励※謇ｱ縺・//

    // 蜷・｡√ｒ謠冗判 //
    for (int i = 0; i < count; i++)
    {
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (result / divisor) % 10;
        m_pSprite->Draw(m_pRankImage.get(),1000 + i * 73,srcY, 68 * num,540,68,103);  // 謨ｰ蟄礼判蜒上ｒ讓ｪ縺ｫ荳ｦ縺ｹ縺ｦ謠冗判 //
    }
}
