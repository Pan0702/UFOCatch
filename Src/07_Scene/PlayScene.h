#pragma once
#include "../04_FrameWork/SceneBase.h"
#include "../05_CommonFile/Object3D.h"
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	void DrawCircle(const VECTOR3& center, float radius, DWORD color);
private:
	bool flag ;
};
