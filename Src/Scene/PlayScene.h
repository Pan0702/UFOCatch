#pragma once
#include "../FrameWork/SceneBase.h"
#include "../Object3D.h"
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	void DrawCircle(const VECTOR3& center, float radius, DWORD color);
};
