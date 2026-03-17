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
#include "../Scene/OIScene.h"
#include "../Scene/NormalScene.h"

std::unique_ptr<SceneBase> SceneFactory::CreateFirst()
{
	SingleInstantiate<CGameInstance>();
	return std::make_unique<TitleScene>();
	
	return nullptr;
}

std::unique_ptr<SceneBase> SceneFactory::Create(const std::string& name)
{

	if (name == "TitleScene") {
		return std::make_unique<TitleScene>();
	}
	if (name == "Easy") {
		return std::make_unique<PlayScene>();
	}
	if (name == "Normal") {
		return std::make_unique<CNormalScene>();
	}
	if (name == "PlayScene") {
		return std::make_unique<PlayScene>();
	}
	if (name == "SelectScene")
	{
		return std::make_unique<CSelectionScene>();
	}
	if (name == "ResultScene")
	{
		return std::make_unique<CResultScene>();
	}
	if (name == "Tutorial")
	{
		return std::make_unique<CTutorialScene>();	
	}
	if (name == "Debug")
	{
		return std::make_unique<CDebugScene>();
	}
	if (name == "OI")
	{
		return std::make_unique<OIScene>();
	}
	//
	assert(false);
	return nullptr;
}
