#pragma once
#include "../Framework/SceneBase.h"
#include "../Utils/WipeAnimator.h"

// 繧ｲ繝ｼ繝襍ｷ蜍墓凾縺ｮ繧ｿ繧､繝医Ν繧ｷ繝ｼ繝ｳ //
class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
private:

	void Update() override;
	void Draw() override;
	std::vector<std::string> m_text;
	int m_selectedIndex;

	// 繧ｿ繧､繝医Ν逕ｻ髱｢縺ｧ陦ｨ遉ｺ縺吶ｋ逕ｻ蜒上・諠・ｱ //
	struct ImageInfo
	{
		VECTOR2 pos;
		VECTOR4 imageSize;
		CSpriteImage* pImage;
		ImageInfo(const VECTOR2& pos, const VECTOR4& imageSize, CSpriteImage* pImage)
			: pos(pos), imageSize(imageSize), pImage(pImage) {}
	};
	std::vector<ImageInfo> m_imageInfos;

	WipeAnimator m_wipeAnim{ 2 };
};

