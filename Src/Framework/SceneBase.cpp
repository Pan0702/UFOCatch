#include "SceneBase.h"
#include "objectManager.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	SAFE_DELETE(m_pUI);
	ObjectManager::ChangeScene();
}

CUIBase* SceneBase::GetUIPtr() const
{
	return m_pUI;
}

