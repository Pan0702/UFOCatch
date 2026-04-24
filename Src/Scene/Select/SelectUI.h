#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButtons.h"
#include "../../UI/UIImage.h"

class CSelectUI : public CUIBase
{
public:
    CSelectUI();

private:
    /// @brief buttonの初期化処理
    void InitButtons();

    /// @brief バックイメージの初期化処理
    void BackImage();

    /// @brief 難易度画像の初期化処理
    void InitDifficultyImage();

    /// @brief フォーカス中のボタンに対応する難易度画像を更新する
    void UpdateDifficultyImage();

    void Update() override;

    CUIButtons m_buttons;
    std::vector<std::string> m_sceneNames;

    CUIImage* m_pDifficultyImage;               // 難易度表示画像ウィジェット
    std::vector<CSpriteImage*> m_difficultyTextures; // N/E/T に対応するテクスチャ
    int m_lastFocusIndex;                        // 前フレームのフォーカスインデックス
};
