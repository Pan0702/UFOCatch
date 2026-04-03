#include "OIScene.h"
#include "../../Framework/GameObject.h"
OIScene::OIScene()
{
    m_pSpriteImage = std::make_unique<CSpriteImage>("data/Select/OI.png");
}

void OIScene::Draw()
{
    CSprite spr;
    spr.Draw(m_pSpriteImage.get(),0,0,0,0,1366,768);
}

void OIScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeSceneWithTransition("TitleScene");
    }
}
