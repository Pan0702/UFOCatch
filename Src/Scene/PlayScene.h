#pragma once
#include "../Framework/SceneBase.h"

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
	CXAudioSource* m_pBGM;
	//4ぶんき計算Debug
	//void QTreeDebug();
	
};
