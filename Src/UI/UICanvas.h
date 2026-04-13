#pragma once
#include "UIWidget.h"

/// @brief 複数のウィジェットを管理し、更新・描画を統括するクラス
/// @details ウィジェットはlayer順にソートして描画される
class CUICanvas
{
public:
    /// @param size キャンバスの論理サイズ（デフォルト: 1366x768）
    CUICanvas(const VECTOR2& size = VECTOR2(1366, 768));
    ~CUICanvas();

    /// @brief ウィジェットを追加して生ポインタを返す
    /// @details 所有権はキャンバスに移る。返されたポインタは所有しない参照として使う。
    template <typename T>
    T* AddWidget(std::unique_ptr<T> widget)
    {
        T* ptr = widget.get();
        m_widgets.push_back(std::move(widget));
        return ptr;
    }

    void RemoveWidget(CUIWidget* widget);

    void Update();
    void Draw();

    /// @brief 全ウィジェットを削除する
    void Clear();
    VECTOR2 GetCanvasSize() const;

private:
    std::vector<std::unique_ptr<CUIWidget>> m_widgets;
    VECTOR2 m_canvasSize;
};
