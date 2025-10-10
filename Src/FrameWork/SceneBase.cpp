#include "SceneBase.h"
#include "../FrameWork/objectManager.h""

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	ObjectManager::ChangeScene();
}
