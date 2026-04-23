#include "PlayerHP.h"
#include "Player.h"
#include <algorithm>
#include "../System/GameInstance.h"

////////////////////
// HPの初期値を指定してインスタンスを生成する
// @param hp 初期HP //
////////////////////
CPlayerHP::CPlayerHP(const int& hp)
    : m_maxHp(hp)
{
    m_currentHp = hp;
    m_found = false;
    m_invincible = 0;
    m_findCount = 0;
    m_findMaxCount = 1.5f;
}

CPlayerHP::~CPlayerHP()
{
}

////////////////////
// HPを減らす処理と目撃カウントを増やす //
////////////////////
void CPlayerHP::SubHP()
{
    if (not m_seemToFind)
    {
        if (not m_found)
        {
            CGameInstance::Get()->AddSaw(1);
            m_seemToFind = true;
        }
    }
}

void CPlayerHP::Update()
{
    if (m_seemToFind)
    {
        m_findCount += SceneManager::DeltaTime();
        if (m_findCount >= m_findMaxCount)
        {
            m_invincible = 0;
            m_found = true;
            m_seemToFind = false;
            m_currentHp--;
            CGameInstance::Get()->AddDiscovery(1);
        }
    }
    else if (m_found)
    {
        m_invincible += SceneManager::DeltaTime();
        if (m_invincible >= m_findMaxCount)
        {
            m_findCount = 0;
            m_found = false;
        }
    }
    else
    {
        m_findCount = (std::max)(m_findCount - SceneManager::DeltaTime(), 0.0f);
    }
    // 死亡したのでシーンを切り替える
    if (m_currentHp <= 0)
    {
        SceneManager::ChangeSceneWithTransition(SceneName::RESULT);
    }
}

void CPlayerHP::ResetFlag()
{
    m_seemToFind = false;
}
