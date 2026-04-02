#include "UIWidget.h"

#include "../Framework/sceneManager.h"

CUIWidget::CUIWidget()
{
    m_anchor = AnchorType::TopLeft;
}

void CUIWidget::RemoveChild(CUIWidget* child)
{
    auto it = std::ranges::find_if(m_children,
                                   [child](const std::unique_ptr<CUIWidget>& ptr)
                                   {
                                       return ptr.get() == child;
                                   });
    if (it != m_children.end())
    {
        m_children.erase(it);
    }
}

void CUIWidget::ClearChild()
{
    m_children.clear();
}

void CUIWidget::Update()
{
    m_pAnimPlayer.Update(SceneManager::DeltaTime());
    ApplayAnimValues();
    if (!m_visible) return;
    for (auto& child : m_children)
    {
        child->Update();
    }
}

void CUIWidget::Draw(CSprite& sprite)
{
    if (!m_visible) return;
    std::vector<CUIWidget*> sortChild;
    for (auto& child : m_children)
    {
        sortChild.push_back(child.get());
    }
    std::ranges::sort(sortChild, [](CUIWidget* a, CUIWidget* b)
    {
        return a->GetLayer() < b->GetLayer();
    });
    for (auto& child : sortChild)
    {
        child->Draw(sprite);
    }
}

void CUIWidget::ApplayAnimValues()
{
    const auto& values = m_pAnimPlayer.GetCurrentValues();
    if (values.contains(AnimatedProperty::ScaleX))
        m_scale.x = values.at(AnimatedProperty::ScaleX);
    
    if (values.contains(AnimatedProperty::ScaleY))
        m_scale.y = values.at(AnimatedProperty::ScaleY);
    
    if (values.contains(AnimatedProperty::Alpha))
        m_alpha = values.at(AnimatedProperty::Alpha);
    
    if (values.contains(AnimatedProperty::PositionX))
        m_position.x = values.at(AnimatedProperty::PositionX);
    
    if (values.contains(AnimatedProperty::PositionY))
        m_position.y = values.at(AnimatedProperty::PositionY);
    
}

void CUIWidget::SetAnchor(AnchorType anchor)
{
    m_anchor = anchor;
}

void CUIWidget::SetPosition(const VECTOR2& pos)
{
    m_position = pos;
}

void CUIWidget::SetSize(const VECTOR2& size)
{
    m_size = size;
}

void CUIWidget::SetLayer(int layer)
{
    m_layer = layer;
}

void CUIWidget::SetAlpha(float alpha)
{
    m_alpha = alpha;
}

void CUIWidget::SetVisible(bool visible)
{
    m_visible = visible;
}

VECTOR2 CUIWidget::GetWorldPosition()
{
    VECTOR2 worldPos = m_position;
    VECTOR2 anchorOffset = CalcAnchorOffset();
    worldPos += anchorOffset;

    if (m_pParent != nullptr)
    {
        VECTOR2 parentPos = m_pParent->GetWorldPosition();
        worldPos += parentPos;
    }

    return worldPos;
}

VECTOR2 CUIWidget::CalcAnchorOffset() const
{
    if (m_pParent == nullptr) return {0, 0};

    VECTOR2 parentSize = m_pParent->GetSize();
    VECTOR2 offset = VECTOR2(0, 0);
    switch (m_anchor)
    {
    case AnchorType::TopLeft:
        offset = VECTOR2(0, 0);
        break;
    case AnchorType::TopCenter:
        offset = VECTOR2(parentSize.x * 0.5f, 0);
        break;
    case AnchorType::TopRight:
        offset = VECTOR2(parentSize.x, 0);
        break;
    case AnchorType::MiddleLeft:
        offset = VECTOR2(0, parentSize.y * 0.5f);
        break;
    case AnchorType::MiddleCenter:
        offset = VECTOR2(parentSize.x * 0.5f, parentSize.y * 0.5f);
        break;
    case AnchorType::MiddleRight:
        offset = VECTOR2(parentSize.x, parentSize.y * 0.5f);
        break;
    case AnchorType::BottomLeft:
        offset = VECTOR2(0, parentSize.y);
        break;
    case AnchorType::BottomCenter:
        offset = VECTOR2(parentSize.x * 0.5f, parentSize.y);
        break;
    case AnchorType::BottomRight:
        offset = VECTOR2(parentSize.x, parentSize.y);
        break;
    }
    
    return offset;
}

const VECTOR2& CUIWidget::GetPosition()
{
    return m_position;
}

const VECTOR2& CUIWidget::GetSize()
{
    return m_size;
}

int CUIWidget::GetLayer() const
{
    return m_layer;
}

float CUIWidget::GetAlpha() const
{
    return m_alpha;
}

bool CUIWidget::IsVisible() const
{
    return m_visible;
}

CUIAnimationPlayer& CUIWidget::GetAnimationPlayer()
{
    return m_pAnimPlayer;
}
