#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "UIAnimationTrack.h"

class CUIAnimation
{
public:
    CUIAnimation(std::string name = "");
    CUIAnimationTrack* AddTrack(AnimatedProperty property);
    void Evaluate(float time, std::unordered_map<AnimatedProperty, float>& outValues) const;
    const std::string& GetName() const;
    void SetName(const std::string& name);

    float GetDuration() const;
    void SetDuration(float duration);
    
    size_t GetTrackCount() const;
    
private:
    std::string m_name;
    std::vector<std::unique_ptr<CUIAnimationTrack>> m_tracks;
    float m_duration = 0.0f;
    
};
