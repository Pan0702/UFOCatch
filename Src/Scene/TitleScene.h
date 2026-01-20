#pragma once
#include "../Framework/SceneBase.h"

// ゲーム起動時のタイトルシーン //
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

	// タイトル画面で表示する画像の情報 //
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
