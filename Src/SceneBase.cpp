#include "SceneBase.h"
#include "ObjectManager.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	ObjectManager::ChangeScene();
}
