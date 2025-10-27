#include "PlayScene.h"
#include <fstream>
#include "../06_GameLib/CsvReader.h"
#include <assert.h>
#include "../09_Enemy/AnimalManager.h"
#include "../08_Player/PCamera.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/EnemyHuman.h"


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
