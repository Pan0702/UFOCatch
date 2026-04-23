#include "UICanvas.h"

CUICanvas::CUICanvas(const VECTOR2& size)
    : m_canvasSize(size)
{
}

CUICanvas::~CUICanvas() = default;

void CUICanvas::RemoveWidget(CUIWidget* widget)
{
    auto it = std::ranges::find_if(m_widgets,
                                   [widget](const std::unique_ptr<CUIWidget>& ptr)
                                   {
                                       return ptr.get() == widget;
                                   });
    if (it != m_widgets.end())
    {
        m_widgets.erase(it);
    }   
}

void CUICanvas::Update()
{
    for (auto& widget : m_widgets)
    {
        widget->Update();
    }  
}

void CUICanvas::Draw()
{
    std::vector<CUIWidget*> sortWidgets;
    for (auto& widget : m_widgets)
    {
        sortWidgets.push_back(widget.get());
    }
    std::ranges::sort(sortWidgets, [](const CUIWidget* a, const CUIWidget* b)
    {
        return a->GetLayer() < b->GetLayer();
    });
    CSprite spr;
    for (auto& widget : sortWidgets)
    {
        widget->Draw(spr);
    }
}

void CUICanvas::Clear()
{
    m_widgets.clear(); 
}

VECTOR2 CUICanvas::GetCanvasSize() const
{
    return m_canvasSize;
}
