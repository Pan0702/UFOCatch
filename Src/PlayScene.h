#pragma once
#include "SceneBase.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
};
