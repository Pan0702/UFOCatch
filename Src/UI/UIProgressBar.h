#pragma once
#include "UIImage.h"

class CUIProgressBar : public CUIWidget
{
public:
    CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill,
               const VECTOR2& pos, const VECTOR4& size);
    
    void SetRatio(float ratio); 
    void Update() override;
private:
    CUIImage* m_pFill       = nullptr;
    CUIImage* m_pBg         = nullptr;
    float     m_maxWidth    = 0.0f;
    float     m_startRatio  = 1.0f;
    float     m_targetRatio = 1.0f;  
    float     m_currentRatio= 1.0f;  
    float     m_easeSpeed   = 3.0f;  
    float     m_t           = 1.0f;            
    float     m_duration    = 0.5f; 
};
