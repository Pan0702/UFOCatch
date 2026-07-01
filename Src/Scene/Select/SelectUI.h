#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButtons.h"
#include "../../UI/UIImage.h"

/// <summary>シーンで使う Select UI の情報と処理をまとめる型</summary>
class CSelectUI : public CUIBase
{
public:
    /// CSelectUI を初期化する
    CSelectUI();

private:
    /// Buttons を初期化する
    void InitButtons();

    /// Back Image の処理を行う
    void BackImage();

    /// Difficulty Image を初期化する
    void InitDifficultyImage();

    /// Difficulty Image を毎フレームの状態を更新する
    void UpdateDifficultyImage();

    /// 毎フレームの状態を更新する
    void Update() override;

    CUIButtons m_buttons;
    std::vector<std::string> m_sceneNames;

    CUIImage* m_pDifficultyImage; // 難易度表示画像ウィジェット
    std::vector<CSpriteImage*> m_difficultyTextures; // N/E/T に対応するテクスチャ
    int m_lastFocusIndex; // 前フレームのフォーカスインデックス
};
