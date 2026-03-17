#pragma once
#include "../Framework/SceneBase.h"

// 繝｡繧､繝ｳ縺ｮ繝励Ξ繧､繧ｷ繝ｼ繝ｳ //
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	// 繝ｪ繧ｶ繝ｫ繝医す繝ｼ繝ｳ縺ｸ驕ｷ遘ｻ縺吶ｋ //
	void ChangeResultScene();
private:
	CXAudioSource* m_pBGM;
	//4縺ｶ繧薙″險育ｮ優ebug
	void QTreeDebug();
	
};
