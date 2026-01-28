#include "Idle.h"

#include "../Human/Human.h"
#include "../System/EnemyRegistr.h"

CIdle::CIdle(CEnemyBase* e,float endFrame)
    :m_endFrame(endFrame)
{
    m_pOwner = e;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
}

void CIdle::Enter()
{
    m_idleState = static_cast<int>(round(Randomf(0, 1)));
    if (m_idleState == ANIMATION)
    {
        m_pOwner->GetAnimator()->MergePlay(AnimationType::A_IDEL);
        m_pOwner->GetAnimator()->SetPlaySpeed(1.0f);
    }
    else
    {
        m_pOwner->GetAnimator()->Stop();
    }
    m_timerCnt = 0;
    m_isFinish = false;
    
    CHuman* human = ObjectManager::FindGameObject<CHuman>();
    if (human == m_pOwner and human != nullptr)
    {
        m_isFinish = true;
    }
}

void CIdle::Update()
{
    switch (m_idleState)
    {
    case ANIMATION:
            IdleAnim();
        break;
    case STOP:
        IdleStop();
        break;
    default:
        assert("error:IdleState");
        break;
    }
}

void CIdle::IdleStop()
{
    m_timerCnt += SceneManager::DeltaTime();
    //1秒になったらシーンを終了
    if (m_timerCnt > 1)
    {
        m_isFinish = true;
    }
}

void CIdle::IdleAnim()
{
    if (AnimationFinish())
    {
        m_isFinish = true;
    }
}

bool CIdle::AnimationFinish() const
{
    if (m_pOwner->GetAnimator()->CurrentFrame() >= 570.0f)
    {
        return true;
    }
    return false;
}
