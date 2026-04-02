#include "UIAnimation.h"

CUIAnimation::CUIAnimation(std::string name)
    : m_name(std::move(name)), m_duration(0.0f)
{
}

CUIAnimationTrack* CUIAnimation::AddTrack(AnimatedProperty property)
{
    auto track = std::make_unique<CUIAnimationTrack>(property);
    CUIAnimationTrack* raw = track.get();
    m_tracks.push_back(std::move(track));
    return raw;
}

void CUIAnimation::Evaluate(float time, std::unordered_map<AnimatedProperty,float>& outValues) const
{
    for (const auto& track : m_tracks)
    {
        const float value = track->Evaluate(time);
        outValues[track->GetPopety()] = value;
    }
}

const std::string& CUIAnimation::GetName() const
{
    return m_name;
}

void CUIAnimation::SetName(const std::string& name)
{
    m_name = name;
}

float CUIAnimation::GetDuration() const
{
    return m_duration;  
}

void CUIAnimation::SetDuration(float duration)
{
    m_duration = duration;   
}

size_t CUIAnimation::GetTrackCount() const
{
    return m_tracks.size();
}
