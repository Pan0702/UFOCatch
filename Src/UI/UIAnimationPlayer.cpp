#include "UIAnimationPlayer.h"

#include <iostream>

CUIAnimationPlayer::CUIAnimationPlayer()
    : m_pCurrentAnim(nullptr), m_currentTime(0.0f)
{
}

void CUIAnimationPlayer::AddAnimation(const std::string& name, std::shared_ptr<CUIAnimation> anim)
{
    anim->SetName(name);
    m_animations[name] = std::move(anim);  
}

void CUIAnimationPlayer::Play(const std::string& name, bool loop)
{
    auto it = m_animations.find(name);
    if (it == m_animations.end()) return;
    m_pCurrentAnim = it->second.get(); 
    m_currentTime = 0.0f;
    m_isPlaying = true;
    m_isPaused = false;
    m_isLoop = loop; 
    
    m_currentValues.clear();
    m_pCurrentAnim->Evaluate(0.0f,m_currentValues);
}

void CUIAnimationPlayer::Stop()
{
    m_isPlaying = false;
    m_isPaused = false;
    m_pCurrentAnim = nullptr;
    m_currentTime = 0.0f;
    m_onComplete = nullptr;
}

void CUIAnimationPlayer::SetOnComplete(std::function<void()> callback)
{
    m_onComplete = std::move(callback);
}

void CUIAnimationPlayer::Pause()
{
    m_isPaused = true;
}

void CUIAnimationPlayer::Resume()
{
    m_isPaused = false;
}

void CUIAnimationPlayer::Update(float deltaTime)
{
    if (!m_isPlaying || m_isPaused || !m_pCurrentAnim) return;

    m_currentTime += deltaTime;
    if (m_currentTime >= m_pCurrentAnim->GetDuration())
    {
        if (m_isLoop)
        {
            // ループ時ははみ出た分を折り返す（fmodで剰余を取る）
            m_currentTime = fmod(m_currentTime, m_pCurrentAnim->GetDuration());
        }
        else
        {
            m_currentTime = m_pCurrentAnim->GetDuration();
            m_isPlaying = false;
            if (m_onComplete)
            {
                // コールバック内でStop()が呼ばれる場合に備え、先にムーブしてからnullptrにする
                auto cb = std::move(m_onComplete);
                m_onComplete = nullptr;
                cb();
            }
        }
    }
    m_currentValues.clear();
    m_pCurrentAnim->Evaluate(m_currentTime, m_currentValues);
}

const std::unordered_map<AnimatedProperty, float>& CUIAnimationPlayer::GetCurrentValues() const
{
    return m_currentValues;
}

bool CUIAnimationPlayer::IsPlaying() const
{
    return m_isPlaying;
}

const std::string& CUIAnimationPlayer::GetCurrentAnimName() const
{
    static std::string empty;
    return m_pCurrentAnim ? m_pCurrentAnim->GetName() : empty;
}
