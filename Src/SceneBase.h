#pragma once

/// <summary>
/// シーンの基底クラス
/// </summary>
/// <author>N.Hanai</author>

#include <list>
#include "sceneManager.h"
#include "../Src/GameMain.h"

class SceneBase {
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Update() {}
	virtual void Draw() {}
};
