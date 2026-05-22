#pragma once
#include "../Common/Object3D.h"

/// <summary>ステージエディタで使う Button の情報と処理をまとめる型</summary>
class Button;
/// <summary>ステージエディタで使う TRS の情報と処理をまとめる型</summary>
class TRS;
/// <summary>ステージエディタで使う Controller の情報と処理をまとめる型</summary>
class Controller;

/// <summary>ステージエディタで使う Editor UI の情報と処理をまとめる型</summary>
class EditorUI : public Object3D
{
public:
    /// EditorUI を初期化する
    EditorUI();
    /// EditorUI の終了処理を行う
    ~EditorUI() = default;

private:
    Button* m_pButton;
    TRS* m_pTRS;
    Controller* m_pController;

    /// 描画する
    void Draw() override;

    /// Editor Tools Window を描画する
    void DrawEditorToolsWindow() const;
    /// Setting Window を描画する
    void DrawSettingWindow() const;
    /// Hierarchy Window を描画する
    void DrawHierarchyWindow() const;
    /// Transform Window を描画する
    void DrawTransformWindow() const;
    /// Move Amount Window を描画する
    void DrawMoveAmountWindow() const;
};
