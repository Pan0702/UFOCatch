#include "PlayScene.h"
#include <fstream>
#include "../GameLib/CsvReader.h"
#include <assert.h>
#include "../Enemy/AnimalManager.h"
#include "../Player/PCamera.h"
#include "../Player/Player.h"


PlayScene::PlayScene()
{
	Instantiate<CPlayer>();
	Instantiate<CPlayerCamera>();
	Instantiate<CAnimalManager>();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void PlayScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		20, 20, "PlayScene", 16, RGB(255, 255, 0));
}
