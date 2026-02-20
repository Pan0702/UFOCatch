#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>

#include "../System/GameInstance.h"
#include "../Scene/TitleScene.h"
#include "../Scene/PlayScene.h"
#include "../Scene/ResultScene.h"
#include "../Scene/SelectionScene.h"
#include "../Scene/TutorialScene.h"
#include "../Enemies/AnimalChicken/Chicken.h"
#include "../Scene/DebugScene.h"

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
	if (name == "Easy") {
		return new PlayScene();
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
	if (name == "Tutorial")
	{
		return new CTutorialScene();
	}
	if (name == "Debug")
	{
		return new CDebugScene();
	}
	//
	assert(false);
	return nullptr;
}
