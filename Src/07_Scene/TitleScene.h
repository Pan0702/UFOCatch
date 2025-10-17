#pragma once
#include "../04_FrameWork/SceneBase.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
private:
	~TitleScene();
	void Update() override;
	void Draw() override;
	CSpriteImage* m_pTitleImage;
};
