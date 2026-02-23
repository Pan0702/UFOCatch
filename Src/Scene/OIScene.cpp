#include "OIScene.h"

OIScene::OIScene()
{
    m_pSpriteImage = new CSpriteImage("data/Select/OI.png");
}

void OIScene::Draw()
{
    CSprite spr;
    spr.Draw(m_pSpriteImage,0,0,0,0,1366,768);
}

void OIScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeSceneWithTransition("TitleScene");
    }
}
