#pragma once

/// <summary>
/// �V�[���̊��N���X
/// </summary>
/// <author>N.Hanai</author>

#include <list>
#include "sceneManager.h"
#include "../03_GameMain/GameMain.h"

class SceneBase {
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Update() {}
	virtual void Draw() {}
};
