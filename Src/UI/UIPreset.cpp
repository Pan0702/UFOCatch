#include "UIPreset.h"

namespace UIPreset
{

TransitionAnim Transition(float duration, std::function<float(float)> easing)
{
    return { duration, std::move(easing) };
}

std::unique_ptr<CUIAnimation> FadeIn(float duration, float from, float to)
{
    auto anim = std::make_unique<CUIAnimation>("FadeIn");
    anim->SetDuration(duration);
    auto* track = anim->AddTrack(AnimatedProperty::Alpha);
    track->AddKeyFrame(0.0f,     from);
    track->AddKeyFrame(duration, to);
    return anim;
}

std::unique_ptr<CUIAnimation> FadeOut(float duration, float from, float to)
{
    auto anim = std::make_unique<CUIAnimation>("FadeOut");
    anim->SetDuration(duration);
    auto* track = anim->AddTrack(AnimatedProperty::Alpha);
    track->AddKeyFrame(0.0f,     from);
    track->AddKeyFrame(duration, to);
    return anim;
}

std::unique_ptr<CUIAnimation> ScaleIn(float duration, float from, float to)
{
    auto anim = std::make_unique<CUIAnimation>("ScaleIn");
    anim->SetDuration(duration);
    auto* trackX = anim->AddTrack(AnimatedProperty::ScaleX);
    auto* trackY = anim->AddTrack(AnimatedProperty::ScaleY);
    trackX->AddKeyFrame(0.0f,     from);
    trackX->AddKeyFrame(duration, to);
    trackY->AddKeyFrame(0.0f,     from);
    trackY->AddKeyFrame(duration, to);
    return anim;
}

std::unique_ptr<CUIAnimation> ScalePop(float duration, float peak)
{
    auto anim = std::make_unique<CUIAnimation>("ScalePop");
    anim->SetDuration(duration);
    const float mid = duration * 0.5f;
    auto* trackX = anim->AddTrack(AnimatedProperty::ScaleX);
    auto* trackY = anim->AddTrack(AnimatedProperty::ScaleY);
    trackX->AddKeyFrame(0.0f,     1.0f);
    trackX->AddKeyFrame(mid,      peak);
    trackX->AddKeyFrame(duration, 1.0f);
    trackY->AddKeyFrame(0.0f,     1.0f);
    trackY->AddKeyFrame(mid,      peak);
    trackY->AddKeyFrame(duration, 1.0f);
    return anim;
}

std::unique_ptr<CUIAnimation> SlideInX(float duration, float fromOffset, float toOffset)
{
    auto anim = std::make_unique<CUIAnimation>("SlideInX");
    anim->SetDuration(duration);
    auto* track = anim->AddTrack(AnimatedProperty::PositionX);
    track->AddKeyFrame(0.0f,     fromOffset);
    track->AddKeyFrame(duration, toOffset);
    return anim;
}

std::unique_ptr<CUIAnimation> SlideInY(float duration, float fromOffset, float toOffset)
{
    auto anim = std::make_unique<CUIAnimation>("SlideInY");
    anim->SetDuration(duration);
    auto* track = anim->AddTrack(AnimatedProperty::PositionY);
    track->AddKeyFrame(0.0f,     fromOffset);
    track->AddKeyFrame(duration, toOffset);
    return anim;
}

std::unique_ptr<CUIAnimation> PopIn(float duration, float fromScale, float fromAlpha)
{
    auto anim = std::make_unique<CUIAnimation>("PopIn");
    anim->SetDuration(duration);
    auto* alpha  = anim->AddTrack(AnimatedProperty::Alpha);
    auto* scaleX = anim->AddTrack(AnimatedProperty::ScaleX);
    auto* scaleY = anim->AddTrack(AnimatedProperty::ScaleY);
    alpha->AddKeyFrame(0.0f,     fromAlpha);
    alpha->AddKeyFrame(duration, 1.0f);
    scaleX->AddKeyFrame(0.0f,     fromScale);
    scaleX->AddKeyFrame(duration, 1.0f);
    scaleY->AddKeyFrame(0.0f,     fromScale);
    scaleY->AddKeyFrame(duration, 1.0f);
    return anim;
}

} // namespace UIPreset