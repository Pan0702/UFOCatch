#include "Find.h"
#include "../System/EnemyManager.h"
#include "../../Player/PlayerHP.h"
#include "../Human/Human.h"

CFind::CFind(CHuman* human)
{
    m_pOwner = human;
}

void CFind::Enter()
{
    AudioManager::Load("HumnaVoice",_T("data/Sound/hey.wav"));
    AudioManager::Play(_T("HumnaVoice"), false);
    m_isFinish = false;
    ObjectManager::FindGameObject<CPlayerHP>()->SubHP();
    m_pOwner->GetAnimator()->MergePlay(A_IDEL);
}

void CFind::Update()
{
    if (not m_pOwner->GetInSight())
    {
        m_pOwner->SetAngle(0);
        m_isFinish = true;
    }
}
