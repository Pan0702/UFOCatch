#pragma once
#include "UIWidget.h"

//複数のWidgetを管理し、更新・描画を統括する//
class CUICanvas
{
public:
    CUICanvas(const VECTOR2& size = VECTOR2(1366, 768));
    ~CUICanvas();

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

    void Clear();
    VECTOR2 GetCanvasSize() const;

private:
    std::vector<std::unique_ptr<CUIWidget>> m_widgets;
    VECTOR2 m_canvasSize;
};
