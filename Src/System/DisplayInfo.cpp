#include "DisplayInfo.h"

#include <algorithm>

#include "Timer.h"
#include "../Player/PlayerHP.h"
#include "../Player/Player.h"

CDisplayInfo::CDisplayInfo()
{
    m_pSprite = new CSprite();
    m_playUIImage = new CSpriteImage("data/PlayUI.png");
    m_expImage = new CSpriteImage("data/PlayUIParts.png");
    m_giwakuImage = new CSpriteImage("data/Circle.png");
    m_pLogo = new CSpriteImage("data/Logo.png");
    // 邨碁ｨ灘､繧ｲ繝ｼ繧ｸ蛻晄悄蛹・
    m_prevProportion = 0;
    m_currentWidth = 0;

    // 逍第ヱ繧ｲ繝ｼ繧ｸ蛻晄悄蛹・
    m_prevGiwakuProportion = 0;
    m_currentAngle = 0;
    m_isCutInDraw = false;
    m_cutInCnt = 0;
    SetDrawOrder(-100);
}

CDisplayInfo::~CDisplayInfo()
{
    SAFE_DELETE(m_pSprite);
    SAFE_DELETE(m_playUIImage);
    SAFE_DELETE(m_expImage);
    SAFE_DELETE(m_giwakuImage);
    SAFE_DELETE(m_pLogo);
}

void CDisplayInfo::Update()
{
    // Lerp譖ｴ譁ｰ
    float nextWidth = m_xpWeightLerp.Update(SceneManager::DeltaTime());

    // Lerp荳ｭ縲√∪縺溘・蛟､縺梧峩譁ｰ縺輔ｌ縺溷ｴ蜷・
    if (m_xpWeightLerp.IsAnimating() || nextWidth != m_currentWidth)
    {
        m_currentWidth = nextWidth;

        // 貅繧ｿ繝ｳ縺ｫ驕斐＠縺溘°繝√ぉ繝・け・医Ξ繝吶Ν繧｢繝・・貍泌・螳御ｺ・凾・・
        if (m_currentWidth >= 1224.0f)
        {
            m_currentWidth -= 1224.0f;
            m_prevProportion = 0; // 谺｡縺ｮExpDraw縺ｧ0縺九ｉ菴吝臆蛻・∈縺ｮLerp繧帝幕蟋九＆縺帙ｋ
            m_xpWeightLerp.ForceSetValue(m_currentWidth);
        }
    }
}

void CDisplayInfo::Draw()
{
    ExpDraw();
    //BaseUI繧呈緒逕ｻ
    m_pSprite->Draw(m_playUIImage, 0, 0, 0, 0, 1366, 768);
    GiwakuDraw();
    TimeDraw();
    HPDraw();
    LvDraw();

    // 繧ｿ繧､繝槭・縺九ｉ繧ｫ繝・ヨ繧､繝ｳ諠・ｱ繧貞叙蠕励＠縺ｦ謠冗判
    CTimer* pTimer = ObjectManager::FindGameObject<CTimer>();
    if (pTimer && pTimer->IsCutInVisible())
    {
        CutIn(pTimer->GetCutInNum());
    }
}


////////////////////
// 逍第ヱ繧ｲ繝ｼ繧ｸ繧呈緒逕ｻ縺吶ｋ //
////////////////////
void CDisplayInfo::GiwakuDraw()
{
    CPlayerHP* pHp = ObjectManager::FindGameObject<CPlayerHP>();
    //蜑ｲ蜷医ｒ險育ｮ・
    float proportion = avoidZero(pHp->GetFindCount() / pHp->GetMaxFindCount());
    //逍第ヱ繧ｲ繝ｼ繧ｸ繧呈緒逕ｻ
    m_pSprite->DrawCircle(m_giwakuImage, 1122, 469, 0, 0, 230, 230, 0.0f, proportion * XM_2PI);

    if (pHp->GetFoundFlag())
    {
        //逍第ヱ縺九ｉ遒ｺ菫｡縺ｫ螟峨ｏ縺｣縺溘→縺阪・隕九◆逶ｮ繧呈緒逕ｻ
        m_pSprite->Draw(m_expImage, 1192, 588, 0, 0, 97, 73);
    }
}

////////////////////
// 邨碁ｨ灘､繧ｲ繝ｼ繧ｸ繧呈緒逕ｻ縺吶ｋ //
////////////////////
void CDisplayInfo::ExpDraw()
{
    //LvBase繧呈緒逕ｻ
    m_pSprite->Draw(m_expImage, 144, 713, 0, 160, 1224, 55);
    CPlayerLevel* pl = ObjectManager::FindGameObject<CPlayerLevel>();
    if (!pl) return;

    //蜑ｲ蜷医ｒ險育ｮ・
    float proportion = avoidZero(pl->GetExp() / pl->GetAllExp());

    static constexpr float epsilon = 0.001f;

    // Lerp縺檎ｵゅｏ縺｣縺ｦ縺・◆繧画眠縺励＞逶ｮ讓吶ｒ險ｭ螳・
    if (!m_xpWeightLerp.IsAnimating())
    {
        // 繝ｬ繝吶Ν繧｢繝・・縺励◆縺九←縺・°・亥牡蜷医′貂帛ｰ代＠縺溘°縲√∪縺溘・1.0繧定ｶ・∴縺溘°・・
        if (proportion < m_prevProportion - epsilon || proportion >= 1.0f)
        {
            // 貅繧ｿ繝ｳ縺ｾ縺ｧLerp縺輔○繧・
            m_xpWeightLerp.Start(m_currentWidth, 1224.0f, 0.5f);
            // m_prevProportion縺ｯUpdate縺ｮ繝ｪ繧ｻ繝・ヨ蜃ｦ逅・〒0縺ｫ縺ｪ繧九・繧貞ｾ・▽
        }
        // 騾壼ｸｸ縺ｮ邨碁ｨ灘､蠅怜刈
        else if (fabs(proportion - m_prevProportion) > epsilon)
        {
            float targetWidth = 1224.0f * proportion;
            m_xpWeightLerp.Start(m_currentWidth, targetWidth, 0.5f);
            m_prevProportion = proportion;
        }
    }

    //LvBar繧呈緒逕ｻ
    m_pSprite->Draw(m_expImage, 144, 721, 0, 100, m_currentWidth, 47);
}

////////////////////
// 繧ｿ繧､繝槭・繧呈緒逕ｻ縺吶ｋ //
////////////////////
void CDisplayInfo::TimeDraw()
{
    CTimer* pTimer = ObjectManager::FindGameObject<CTimer>();
    if (!pTimer) return;

    int time = static_cast<int>(pTimer->GetTime());

    // 譯∵焚繧定ｨ育ｮ・
    int count = 0;
    int tmp = time;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    count = (std::max)(count, 2);

    // 蜷・｡√ｒ蟾ｦ縺九ｉ鬆・↓謠冗判
    for (int i = 0; i < count; i++)
    {
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (time / divisor) % 10;
        m_pSprite->Draw(m_expImage, 230 + i * 58, 640, 68 * num, 540, 68, 103, 55, 67);
    }
}

void CDisplayInfo::HPDraw()
{
    CPlayerHP* pHp = ObjectManager::FindGameObject<CPlayerHP>();
    int currentHp = pHp->GetHP();
    int maxHp = pHp->GetMaxHP();
    m_pSprite->Draw(m_expImage, 1195, 530, 68 * currentHp, 540, 68, 103, 32, 54);
    m_pSprite->Draw(m_expImage, 1247, 530, 68 * maxHp, 540, 68, 103, 32, 54);
}

void CDisplayInfo::LvDraw()
{ 

    CPlayerLevel* pl = ObjectManager::FindGameObject<CPlayerLevel>();
    int lv = pl->GetLv();
    
    // 譯∵焚繧定ｨ育ｮ・
    int count = 0;
    int tmp = lv;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    count = (std::max)(count, 1);

    static constexpr float centerX = 50;
    float halfWidth = count * 33;  
    
    for (int i = 0; i < count; i++)
    {
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (lv / divisor) % 10;
        m_pSprite->Draw(m_expImage, (83 - halfWidth) + i * 64, 666, 68 * num, 540, 68, 103, 64, 80);                                                                               
    }
}

void CDisplayInfo::CutIn(int num)
{
    m_pSprite->Draw(m_pLogo, 400, 185, 0, 141 * num, 605, 141);
}
