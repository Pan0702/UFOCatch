#include "SceneBase.h"
#include "objectManager.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	ObjectManager::ChangeScene();
}
