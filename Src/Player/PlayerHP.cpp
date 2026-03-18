#include "PlayerHP.h"
#include "Player.h"
#include "../System/GameInstance.h"
#include "../Scene/PlayScene.h"

////////////////////
// HP縺ｮ蛻晄悄蛟､繧呈欠螳壹＠縺ｦ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
// @param hp 蛻晄悄HP //
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
// HP繧呈ｸ帙ｉ縺吝・逅・→逶ｮ謦・き繧ｦ繝ｳ繝医ｒ蠅励ｄ縺・//
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
    //逍第ヱ縺ｧ縺ｿ縺､縺九▲縺溘°
    if (m_seemToFind)
    {
        m_findCount += SceneManager::DeltaTime();
        //荳螳壽凾髢薙＠縺九＞縺ｫ縺・◆縺ｮ縺ｧ遒ｺ菫｡縺ｫ螟牙喧
        if (m_findCount >= m_findMaxCount)
        {
            m_invincible = 0;
            m_found = true;
            m_seemToFind = false;
            //縺・∪縺ｮHP縺九ｉ・大ｼ輔￥
            m_currentHp--;
            //隕九▽縺九▲縺溷屓謨ｰ繧定ｿｽ蜉
            pGI->AddDiscovery(1);
        }
    }
    //隕九▽縺九▲縺溘°
    if (m_found)
    {
        m_invincible += SceneManager::DeltaTime();
        //1.5s邨碁℃縺励◆繧臥┌謨ｵ譎る俣繧定ｧ｣髯､
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
    //豁ｻ莠｡縺励◆縺ｮ縺ｧ繧ｷ繝ｼ繝ｳ繧貞・繧頑崛縺・
    if (m_currentHp <= 0)
    {
        SceneManager::ChangeSceneWithTransition("ResultScene");
    }
}

void CPlayerHP::ResetFlag()
{
    m_seemToFind = false;
}

