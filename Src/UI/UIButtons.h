#pragma once
#include "UIButton.h"
#include "UIImage.h"

class UIButtons
{
public:
    UIButtons();
    void Add(CUIButton* b , std::function<void()> onConfirm = nullptr);
    void SetAnim(std::unique_ptr<CUIAnimation> Foucus,std::unique_ptr<CUIAnimation> unFocus) const;
    void MoveFocus(int delta);
    void Confirm() const;
    CUIButton* GetFocus() const;
    
private:
    struct Entry
    {
        CUIButton* pButton;
        std::function<void()> onConfirm;
    };
    std::vector<Entry> m_buttons;
    int m_focusIndex;
};
