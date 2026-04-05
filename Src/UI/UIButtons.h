#pragma once
#include "UIBase.h"
#include "UIButton.h"
#include "UIImage.h"

class CUIButtons
{
public:
    CUIButtons();
    void AddButton(CUIBase* ui, const std::string& name, const VECTOR2& pos);
    void SetAnim(const std::shared_ptr<CUIAnimation>& Foucus, const std::shared_ptr<CUIAnimation>& unFocus) const;
    void MoveFocus(int n);
    void SetImageSize(const VECTOR4& size);
    void SetFocus(int index);
    
private:
    std::vector<CUIButton*> m_buttons;
    int m_focusIndex;
    VECTOR4 m_imageSize;
};
