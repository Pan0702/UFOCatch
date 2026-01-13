#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>

#include "../GameInstance.h"
#include "../07_Scene/LevelSelectionScene.h"
#include "../07_Scene/TitleScene.h"
#include "../07_Scene/PlayScene.h"
#include "../07_Scene/ResultScene.h"
#include "../07_Scene/SelectionScene.h"
#include "../07_Scene/TutorialScene.h"
#include "../12_Tutorial/Tutorial.h"

SceneBase* SceneFactory::CreateFirst()
{
	SingleInstantiate<CGameInstance>();
	return new TitleScene();
	
	return nullptr;
}

SceneBase* SceneFactory::Create(const std::string& name)
{

	if (name == "TitleScene") {
		return new TitleScene();
	}
	if (name == "PlayScene") {
		return new PlayScene();
	}
	if (name == "SelectScene")
	{
		return new CSelectionScene();
	}
	if (name == "ResultScene")
	{
		return new CResultScene();
	}
	if (name == "TutorialScene")
	{
		return new CTutorialScene();
	}
	
	assert(false);
	return nullptr;
}
