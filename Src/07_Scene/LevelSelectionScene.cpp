#include "LevelSelectionScene.h"

namespace
{
    const VECTOR2 IMAGE_SIZE = VECTOR2(680, 190);
}
void CLevelSelectionScene::InitImage()
{
    // 実際のボタン画像を保持する配列
    m_pLevelImages.reserve(3);
    m_pLevelImages.emplace_back(new CSpriteImage("data/Button1.png"));
    m_pLevelImages.emplace_back(new CSpriteImage("data/Button2.png"));
    m_pLevelImages.emplace_back(new CSpriteImage("data/Button3.png"));
    
}

CLevelSelectionScene::CLevelSelectionScene()
{
    m_selectedIndex = 0;
    m_animationOffset = 0.0f;
    m_targetOffset = 0.0f;
    InitImage();
}

CLevelSelectionScene::~CLevelSelectionScene()
{
    // オリジナル画像のみ削除
    for (auto& p : m_pLevelImages)
    {
        delete p;
        p = nullptr;
    }
    // m_pLevelImagesは参照のみなので削除不要
}



void CLevelSelectionScene::Update()
{
    CDirectInput* pDI = GameDevice()->m_pDI;
    if (pDI->CheckKey(KD_TRG, DIK_W) || pDI->CheckKey(KD_TRG,DIK_UP))
    {
        m_selectedIndex = (m_selectedIndex - 1 + m_pLevelImages.size()) % m_pLevelImages.size();
        m_targetOffset = m_selectedIndex * 200.0f; // アニメーション目標値更新
    }
    if (pDI->CheckKey(KD_TRG, DIK_S) || pDI->CheckKey(KD_TRG, DIK_DOWN))
    {
        m_selectedIndex = (m_selectedIndex + 1) % m_pLevelImages.size();
        m_targetOffset = m_selectedIndex * 200.0f; // アニメーション目標値更新
    }
    
    // 滑らかなアニメーション
    float animationSpeed = 0.2f;
    m_animationOffset += (m_targetOffset - m_animationOffset) * animationSpeed;
    
    if (pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeScene("SelectScene");
    }
}

void CLevelSelectionScene::Draw()
{
    CSprite spr;
    
    int centerY = 600;
    int itemSpacing = 200;
    
    for (int i = 0; i < m_pLevelImages.size(); i++)
    {
        if (m_pLevelImages[i] != nullptr)
        {
            // アニメーションオフセットを考慮した位置計算
            float relativePos = i - (m_animationOffset / itemSpacing);
            int yPos = centerY + (int)(relativePos * itemSpacing);
            
            if (yPos > -100 && yPos < 1400)
            {
                // 中央に近いアイテムほど大きく表示
                float distanceFromCenter = abs(relativePos);
                float scale = max(0.7f, 1.0f - distanceFromCenter * 0.3f);
                
                int width = (int)(680 * scale);
                int height = (int)(160 * scale);
                int xPos = 400 - width / 2; // 中央揃え
                
                spr.Draw(m_pLevelImages[i], xPos, yPos, 0, 0, IMAGE_SIZE.x, IMAGE_SIZE.y,IMAGE_SIZE.x,IMAGE_SIZE.y);
            }
        }
    }
    
    GameDevice()->m_pFont->Draw(
        20, 20, "LevelSelect", 16, RGB(255, 0, 0));
}