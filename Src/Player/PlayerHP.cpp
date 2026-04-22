#include "PlayerHP.h"
#include "Player.h"
#include "../System/GameInstance.h"
#include "../Scene/Play/PlayScene.h"

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
    if (!m_pUI)
        m_pUI = static_cast<CPlayUI*>(SceneManager::GetUIPtr());
    // 疑惑でみつかったか
    if (m_seemToFind)
    {
        m_findCount += SceneManager::DeltaTime();
        m_pUI->GetGauge()->SetAlpha(m_findCount / m_findMaxCount);
        // 一定時間視界にいたので確信に変化
        if (m_findCount >= m_findMaxCount)
        {
            m_invincible = 0;
            m_found = true;
            m_seemToFind = false;
            // いまのHPから1引く
            m_currentHp--;
            // 見つかった回数を追加
            CGameInstance::Get()->AddDiscovery(1);
        }
    }
    // 見つかったか（確信状態）
    if (m_found)
    {
        m_pUI->GetGauge()->SetAlpha(0.0f);
        m_invincible += SceneManager::DeltaTime();
        // 1.5s経過したら無敵時間を解除
        if (m_invincible >= m_findMaxCount)
        {
            m_findCount = 0;
            m_found = false;
        }
    }
    if (not m_seemToFind)
    {
        m_pUI->GetGauge()->SetAlpha(m_findCount / m_findMaxCount);
        if (m_findCount >= 0)
        {
            m_findCount -= SceneManager::DeltaTime();
        }
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
