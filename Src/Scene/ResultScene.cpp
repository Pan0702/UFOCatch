#include "ResultScene.h"

#include "../System/GameInstance.h"
#include "../Framework/ObjectManager.h"

CResultScene::CResultScene()
{
    m_pSprite = new CSprite;
    m_pResultImage = new CSpriteImage("data/ScoreBG.jpg");
    m_pRankImage = new CSpriteImage("data/PlayUIParts.png");
    m_pGI = ObjectManager::FindGameObject<CGameInstance>();
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
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeScene("TitleScene");
    }
   // CalcRank();
}


void CResultScene::Draw()
{
    m_pSprite->Draw(m_pResultImage, 0, 0, 0, 0, 1366, 768);  // 画面サイズ全体に背景を描画 //
    CalcRank();
    DrawRank();
    DrawResultNum(m_pGI->GetDiscovery(),60);    // 発見数のY座標 //
    DrawResultNum(m_pGI->GetSaw(),217);         // 目撃数のY座標 //
    DrawResultNum(m_pGI->GetCapture(),387);     // 捕獲数のY座標 //
}

////////////////////
// スコアに基づいてランクを計算する //
////////////////////
void CResultScene::CalcRank()
{
    m_score = m_pGI->GetScore()  - (m_pGI->GetDiscovery() * 3) - (m_pGI->GetSaw());  // ペナルティを差し引いた最終スコア //
    float ratio = avoidZero(static_cast<float>(m_score) / static_cast<float>(m_pGI->GetMaxScore()));
    if (ratio < 0.30f)  // 30%未満でランクD //
    {
        m_rankImageNum = 3;
    }else if (ratio < 0.60f)  // 30%未満でランクC //
    {
        m_rankImageNum = 2;
    }else if (ratio < 0.90f)  // 90%未満でランクB //
    {
        m_rankImageNum = 1;
    }else if (ratio <= 1.0f)  // 90%以上でランクA //
    {
        m_rankImageNum = 0;
    }

}

////////////////////
// ランク画像を描画する //
////////////////////
void CResultScene::DrawRank() const
{
    m_pSprite->Draw(m_pRankImage,270,340,180 * m_rankImageNum,  // ランクごとに横にずらした画像を描画 //
        230,179,308);
}

////////////////////
// リザルト数値を描画する
// @param result 表示する数値
// @param srcY 描画するY座標 //
////////////////////
void CResultScene::DrawResultNum(int result,int srcY)
{
    // 桁数を計算 //
    int count = 0;
    int tmp = result;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    if (count == 0) count = 1;  // 0の場合は1桁として扱う //

    // 各桁を描画 //
    for (int i = 0; i < count; i++)
    {
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (result / divisor) % 10;
        m_pSprite->Draw(m_pRankImage,1000 + i * 73,srcY, 68 * num,540,68,103);  // 数字画像を横に並べて描画 //
    }
}
