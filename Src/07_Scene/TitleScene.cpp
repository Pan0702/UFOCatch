#include "TitleScene.h"
#include "../03_GameMain/GameMain.h"

TitleScene::TitleScene()
{
	m_pTitleImage = new CSpriteImage("data/Title.png");
}

TitleScene::~TitleScene()
{
	if (m_pTitleImage != nullptr)
	{
		delete m_pTitleImage;
		m_pTitleImage = nullptr; 
	}
}

void TitleScene::Update()
{
	if (GameDevice()->m_pDI->
			CheckKey(KD_TRG, DIK_P)) {
		SceneManager::ChangeScene("PlayScene");
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
	{
		SceneManager::ChangeScene("ResultScene");
	}
}

void TitleScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		20, 20, "TitleScene", 16, RGB(255, 0, 0));
	CSprite spr;
	spr.Draw(m_pTitleImage,0,0,0,0,1920,1080);
}