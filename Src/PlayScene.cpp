#include "PlayScene.h"
#include <fstream>
#include "CsvReader.h"
#include <assert.h>
#include "AnimalManager.h"
#include "PCamera.h"
#include "Player.h"

PlayScene::PlayScene()
{
	new CPlayer();
	new CPlayerCamera();
	new CAnimalManager();
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
	CSprite spr;
	spr.DrawLine3D(VECTOR3(-10, 0, 0), VECTOR3(10, 0, 0), RGB(255, 0, 0));
	// 円を線で描画
}
