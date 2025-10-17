#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>

#include "../07_Scene/LevelSelectionScene.h"
#include "../07_Scene/TitleScene.h"
#include "../07_Scene/PlayScene.h"

SceneBase* SceneFactory::CreateFirst()
{
	return new TitleScene();
	return nullptr;
}

SceneBase * SceneFactory::Create(const std::string & name)
{
	if (name == "TitleScene") {
		return new TitleScene();
	}
	if (name == "PlayScene") {
		return new PlayScene();
	}
	if (name == "SelectScene")
	{
		return new CLevelSelectionScene();
	}
	
	assert(false);
	return nullptr;
}
