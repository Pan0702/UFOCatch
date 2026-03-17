#pragma once

/// <summary>
/// ・ｽV・ｽ[・ｽ・ｽ・ｽﾌ奇ｿｽ・ｽN・ｽ・ｽ・ｽX
/// </summary>
/// <author>N.Hanai</author>

#include <list>
#include "sceneManager.h"
#include "../Core/Game/GameMain.h"

class SceneBase {
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Update() {}
	virtual void Draw() {}
};
