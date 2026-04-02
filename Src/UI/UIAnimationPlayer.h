#pragma once
#include <memory>
#include <string>

#include "UIAnimation.h"

class CUIAnimationPlayer
{
public:
    CUIAnimationPlayer();
    void AddAnimation(const std::string& name,std::unique_ptr<CUIAnimation> anim);
    
    void Play(const std::string& name,bool loop);
    
    void Stop();
    
    void Pause();
    void Resume();
    
    void Update(float deltaTime);

    const std::unordered_map<AnimatedProperty, float>& GetCurrentValues() const;
    bool IsPlaying() const;
    const std::string& GetCurrentAnimName() const;
    
private:
    std::unordered_map<AnimatedProperty, float> m_currentValues;
    std::unordered_map<std::string, std::unique_ptr<CUIAnimation>> m_animations;
    CUIAnimation* m_pCurrentAnim = nullptr;
    float m_currentTime;  
    bool m_isPlaying = false;
    bool m_isLoop = false;
    bool m_isPaused = false;
};
