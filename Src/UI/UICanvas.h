#pragma once
#include "UIWidget.h"

//複数のWidgetを管理し、更新・描画を統括する//
class UICanvas
{
public:
    UICanvas(const VECTOR2& size = VECTOR2(1366, 768));
    ~UICanvas();

    template <typename T>
    T* AddWidget(std::unique_ptr<T> widget);

    void RemoveWidget(UIWidget* widget);

    void Update();
    void Draw();

    void Clear();
    VECTOR2 GetCanvasSize() const;

private:
    std::vector<std::unique_ptr<UIWidget>> m_widgets;
    VECTOR2 m_canvasSize;
};
