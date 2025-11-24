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
	std::vector<std::string> m_text;
	int m_selectedIndex;

	struct ImageInfo
	{
		VECTOR2 pos;
		VECTOR4 imageSize;
		CSpriteImage* pImage;
		ImageInfo(const VECTOR2& pos, const VECTOR4& imageSize, CSpriteImage* pImage)
			: pos(pos), imageSize(imageSize), pImage(pImage) {}
	};
	std::vector<ImageInfo> m_imageInfos;
};
