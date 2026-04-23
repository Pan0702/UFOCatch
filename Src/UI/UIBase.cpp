#include "UIBase.h"

CUICanvas& CUIBase::GetCanvas()
{
    return m_canvas;
}

void CUIBase::Update()
{
    m_canvas.Update();
}

void CUIBase::Draw()
{
    m_canvas.Draw();
}
