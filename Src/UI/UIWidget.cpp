#include "UIWidget.h"

UIWidget::UIWidget()
{
    m_anchor = AnchorType::TopLeft;
}

template <typename T>
T* UIWidget::AddChild(std::unique_ptr<T> child)
{
    T* ptr = child.get();
    ptr->m_parent = this;
    m_children.push_back(std::move(child));
    return ptr;
}

void UIWidget::RemoveChild(UIWidget* child)
{
    auto it = std::find(m_children.begin(), m_children.end(),
                        [child](std::unique_ptr<UIWidget>& ptr)
                        {
                            return ptr.get() == child;
                        });
    if (it != m_children.end())
    {
        m_children.erase(it);
    }
}

void UIWidget::ClearChild()
{
    m_children.clear();
}

void UIWidget::Update()
{
    if (!m_visible) return;
    for (auto& child : m_children)
    {
        child->Update();
    }
}

void UIWidget::Draw(CSprite& sprite)
{
    if (!m_visible) return;
    std::vector<UIWidget*> sortChild;
    for (auto& child : m_children)
    {
        sortChild.push_back(child.get());
    }
    std::ranges::sort(sortChild, [](UIWidget* a, UIWidget* b)
    {
        return a->GetLayer() < b->GetLayer();
    });
    for (auto& child : sortChild)
    {
        child->Draw(sprite);
    }
}

void UIWidget::SetAnchor(AnchorType anchor)
{
    m_anchor = anchor;
}

void UIWidget::SetPosition(const VECTOR2& pos)
{
    m_position = pos;
}

void UIWidget::SetSize(const VECTOR2& size)
{
    m_size = size;
}

void UIWidget::SetLayer(int layer)
{
    m_layer = layer;
}

void UIWidget::SetAlpha(float alpha)
{
    m_alpha = alpha;
}

void UIWidget::SetVisible(bool visible)
{
    m_visible = visible;
}

VECTOR2 UIWidget::GEtWorldPosition()
{
    VECTOR2 worldPos = m_position;
    VECTOR2 anchorOffset = CalcAnchorOffset();
    worldPos += anchorOffset;

    if (m_pParent != nullptr)
    {
        VECTOR2 parentPos = m_pParent->GEtWorldPosition();
        worldPos += parentPos;
    }

    return worldPos;
}

VECTOR2 UIWidget::CalcAnchorOffset()
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

const VECTOR2& UIWidget::GetPosition()
{
    return m_position;
}

const VECTOR2& UIWidget::GetSize()
{
    return m_size;
}

int UIWidget::GetLayer() const
{
    return m_layer;
}

float UIWidget::GetAlpha() const
{
    return m_alpha;
}

bool UIWidget::IsVisible() const
{
    return m_visible;
}
