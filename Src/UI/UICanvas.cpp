#include "UICanvas.h"

UICanvas::UICanvas(const VECTOR2& size)
    : m_canvasSize(size)
{
}

UICanvas::~UICanvas() = default;

template <typename T>
T* UICanvas::AddWidget(std::unique_ptr<T> widget)
{
    T* ptr = widget.get();
    m_widgets.push_back(std::move(widget));
    return ptr;
}

void UICanvas::RemoveWidget(UIWidget* widget)
{
    auto it = std::ranges::find_if(m_widgets,
                                   [widget](const std::unique_ptr<UIWidget>& ptr)
                                   {
                                       return ptr.get() == widget;
                                   });
    if (it != m_widgets.end())
    {
        m_widgets.erase(it);
    }   
}

void UICanvas::Update()
{
    for (auto& widget : m_widgets)
    {
        widget->Update();
    }  
}

void UICanvas::Draw()
{
    std::vector<UIWidget*> sortWidgets;
    for (auto& widget : m_widgets)
    {
        sortWidgets.push_back(widget.get());
    }
    std::ranges::sort(sortWidgets, [](UIWidget* a, UIWidget* b)
    {
        return a->GetLayer() < b->GetLayer();
    });
    CSprite spr;
    for (auto& widget : sortWidgets)
    {
        widget->Draw(spr);
    }
}

void UICanvas::Clear()
{
    m_widgets.clear(); 
}

VECTOR2 UICanvas::GetCanvasSize() const
{
    return m_canvasSize;
}
