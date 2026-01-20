#pragma once
#include "../04_FrameWork/SceneBase.h"
#include "../05_CommonFile/Object3D.h"

// メインのプレイシーン //
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	// リザルトシーンへ遷移する //
	void ChangeResultScene();
private:
	
};
