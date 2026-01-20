#include "PolayerHP.h"
#include "Player.h"
#include "../System/GameInstance.h"
#include "../Scene/PlayScene.h"

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
            ObjectManager::FindGameObject<CGameInstance>()->AddSaw(1);
            m_seemToFind = true;
        }
    }
}

void CPlayerHP::Update()
{
    CGameInstance* pGI = ObjectManager::FindGameObject<CGameInstance>();
    //疑惑でみつかったか
    if (m_seemToFind)
    {
        m_findCount += SceneManager::DeltaTime();
        //一定時間しかいにいたので確信に変化
        if (m_findCount >= m_findMaxCount)
        {
            m_invincible = 0;
            m_found = true;
            m_seemToFind = false;
            m_currentHp -= 1;
            pGI->AddDiscovery(1);
        }
    }
    //見つかったか
    if (m_found)
    {
        m_invincible += SceneManager::DeltaTime();
        //1.5s経過したら無敵時間を解除
        if (m_invincible >= 1.5f)
        {
            m_findCount = 0;
            m_found = false;
        }
    }
    if (not m_seemToFind)
    {
        if (m_findCount >= 0)
        {
            m_findCount -= SceneManager::DeltaTime();
        }
    }
    //死亡したのでシーンを切り替え
    if (m_currentHp <= 0)
    {
        ObjectManager::FindGameObject<PlayScene>()->ChangeResultScene();
    }
}

void CPlayerHP::ResetFlag()
{
    m_seemToFind = false;
}
