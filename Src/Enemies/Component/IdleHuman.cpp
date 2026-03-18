#include "IdleHuman.h"

#include "../Human/Human.h"

////////////////////
// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
// @param e 縺薙・繧ｳ繝ｳ繝昴・繝阪Φ繝医・謇譛芽・ｼ井ｺｺ髢薙く繝｣繝ｩ繧ｯ繧ｿ繝ｼ・・
////////////////////
CIdleHuman::CIdleHuman(CHuman* e)
{
    m_pOwner = e;
}


////////////////////
// 蠕・ｩ溽憾諷九↓蜈･繧区凾縺ｮ蛻晄悄蛹門・逅・
// 繝ｩ繝ｳ繝繝縺ｧ縲碁壼ｸｸ縺ｮ蠕・ｩ溘い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縲阪°縲悟捉繧翫ｒ隕区ｸ｡縺吶い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縲阪ｒ驕ｸ謚槭☆繧・
////////////////////
void CIdleHuman::Enter()
{
    // 鬥悶・隗貞ｺｦ縺ｨ邨碁℃譎る俣繧貞・譛溷喧 //
    currentAngle = 0;
    animationTime = 0;
    m_isFinish = false;

    // 0縺・縺ｮ繝ｩ繝ｳ繝繝縺ｪ蛟､縺ｧ蠕・ｩ溘い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ遞ｮ鬘槭ｒ豎ｺ螳・//
    m_idleState = static_cast<int>(std::round(Randomf(0, 1)));
    if (m_idleState)
    {
        // 騾壼ｸｸ縺ｮ蠕・ｩ溘い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧貞・逕・//
        m_pOwner->GetAnimator()->MergePlay(A_IDEL);
    }
    else
    {
        // 蜻ｨ繧翫ｒ隕区ｸ｡縺吶い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧貞・逕・//
        m_pOwner->GetAnimator()->MergePlay(A_SEACH);
    }
}

////////////////////
// 豈弱ヵ繝ｬ繝ｼ繝蜻ｼ縺ｰ繧後ｋ譖ｴ譁ｰ蜃ｦ逅・
// 蝓ｺ蠎輔け繝ｩ繧ｹ縺ｮ譖ｴ譁ｰ蜃ｦ逅・ｒ蜻ｼ縺ｳ蜃ｺ縺・
////////////////////
void CIdleHuman::Update()
{
    switch (m_idleState)
    {
    case ANIMATION: IdleAnim();
        break;
    case SEARCH: IdleSearch();
        break;
    default: assert("error:idleHumanState");
        break;
    }
}

////////////////////
// 蠕・ｩ溽憾諷九ｒ邨ゆｺ・☆繧区凾縺ｮ蜃ｦ逅・
// 蝓ｺ蠎輔け繝ｩ繧ｹ縺ｮ邨ゆｺ・・逅・ｒ蜻ｼ縺ｳ蜃ｺ縺・
////////////////////
void CIdleHuman::Exit()
{
    CComponentBase::Exit();
}

////////////////////
// 騾壼ｸｸ縺ｮ蠕・ｩ溘い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蜃ｦ逅・
// ・育樟蝨ｨ縺ｯ譛ｪ螳溯｣・ｼ・
////////////////////
void CIdleHuman::IdleAnim()
{
    if (AnimationFinish())
    {
        m_isFinish = true;
    }
}

////////////////////
// 蜻ｨ繧翫ｒ隕区ｸ｡縺吶い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ邨ゆｺ・愛螳・
// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺檎ｵゆｺ・＠縺溘ｉ鬥悶・隗貞ｺｦ繧偵Μ繧ｻ繝・ヨ縺励∝ｾ・ｩ溽憾諷九ｒ邨ゆｺ・☆繧・
////////////////////
void CIdleHuman::IdleSearch()
{
    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺檎ｵゆｺ・＠縺溘°遒ｺ隱・//
    if (m_pOwner->GetAnimator()->Finished())
    {
        // 鬥悶・隗貞ｺｦ繧呈ｭ｣髱｢・・蠎ｦ・峨↓繝ｪ繧ｻ繝・ヨ //
        m_pOwner->SetAngle(0);

        // 蠕・ｩ溽憾諷九ｒ邨ゆｺ・//
        m_isFinish = true;
    }
}

////////////////////
// 莠ｺ髢薙く繝｣繝ｩ繧ｯ繧ｿ繝ｼ逕ｨ縺ｮ縲悟捉繧翫ｒ隕区ｸ｡縺吶阪い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧貞ｮ溯｡後☆繧・
// 鬥悶ｒ蟾ｦ蜿ｳ縺ｫ蝗櫁ｻ｢縺輔○縺ｪ縺後ｉ縲√い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繝輔Ξ繝ｼ繝縺ｫ蠢懊§縺溯ｧ貞ｺｦ繧定ｨ育ｮ励☆繧・
// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蜈ｨ菴薙・100繝輔Ξ繝ｼ繝縺ｧ讒区・縺輔ｌ縲∽ｻ･荳九・鬆・ｺ上〒蜍穂ｽ懊☆繧具ｼ・
// 1. 蠕・ｩ滂ｼ・-6繝輔Ξ繝ｼ繝・・
// 2. 蟾ｦ繧定ｦ九ｋ・・-21繝輔Ξ繝ｼ繝・壼屓霆｢・俄・ 菫晄戟・・2-33繝輔Ξ繝ｼ繝・俄・ 豁｣髱｢縺ｫ謌ｻ繧具ｼ・4-47繝輔Ξ繝ｼ繝・・
// 3. 蜿ｳ繧定ｦ九ｋ・・8-62繝輔Ξ繝ｼ繝・壼屓霆｢・俄・ 菫晄戟・・3-74繝輔Ξ繝ｼ繝・俄・ 豁｣髱｢縺ｫ謌ｻ繧具ｼ・5-88繝輔Ξ繝ｼ繝・・
// 4. 邨ゆｺ・ｼ・9-100繝輔Ξ繝ｼ繝・・
////////////////////
void CIdleHuman::LookAroundAnim()
{
    float currentFrame = GetCurrentFrame();

    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蜈ｨ菴薙・邱上ヵ繝ｬ繝ｼ繝謨ｰ //
    constexpr float TOTAL_FRAMES = 100.0f;
    if (currentFrame >= TOTAL_FRAMES)
    {
        currentAngle = 0.0f;
        return;
    }

    // 鬥悶・蝗櫁ｻ｢繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺檎ｵゆｺ・☆繧九ヵ繝ｬ繝ｼ繝逡ｪ蜿ｷ・医◎繧御ｻ･髯阪・蠕・ｩ溽憾諷具ｼ・//
    constexpr float NECK_ANIMATION_END = 86.0f;
    if (currentFrame > NECK_ANIMATION_END)
    {
        return;
    }

    // 鬥悶・蝗櫁ｻ｢縺碁幕蟋九＆繧後ｋ縺ｾ縺ｧ縺ｮ蠕・ｩ溘ヵ繝ｬ繝ｼ繝謨ｰ //
    constexpr float WAIT_START_FRAME = 6.0f;
    if (currentFrame <= WAIT_START_FRAME)
    {
        currentAngle = 0.0f;
    }
    else
    {
        // 蠕・ｩ溘ヵ繝ｬ繝ｼ繝繧帝勁縺・◆螳滄圀縺ｮ蜍穂ｽ懊ヵ繝ｬ繝ｼ繝謨ｰ繧定ｨ育ｮ・//
        float cycleFrame = currentFrame - WAIT_START_FRAME;

        // 蟾ｦ蜿ｳ繧定ｦ句屓縺・繧ｵ繧､繧ｯ繝ｫ縺ｮ蜷郁ｨ医ヵ繝ｬ繝ｼ繝謨ｰ・亥ｷｦ竊呈ｭ｣髱｢竊貞承竊呈ｭ｣髱｢・・//
        constexpr float CYCLE_DURATION = 81.0f;

        if (cycleFrame <= CYCLE_DURATION)
        {
            // 蜊翫し繧､繧ｯ繝ｫ・育援蛛ｴ繧定ｦ九ｋ蜍穂ｽ懷・菴難ｼ峨・繝輔Ξ繝ｼ繝謨ｰ //
            constexpr float HALF_CYCLE_DURATION = 41.0f;
            // 迴ｾ蝨ｨ縺悟ｷｦ蜿ｳ縺ｩ縺｡繧峨・繧ｵ繧､繧ｯ繝ｫ縺九ｒ蛻､螳夲ｼ・:蟾ｦ縲・:蜿ｳ・・//
            int halfCycle = static_cast<int>((cycleFrame - 1.0f) / HALF_CYCLE_DURATION);
            // 蜷・濠繧ｵ繧､繧ｯ繝ｫ蜀・〒縺ｮ繝ｭ繝ｼ繧ｫ繝ｫ繝輔Ξ繝ｼ繝逡ｪ蜿ｷ・・-40・・//
            float localFrame = fmodf(cycleFrame - 1.0f, HALF_CYCLE_DURATION);
            // 鬥悶ｒ蟾ｦ蜿ｳ縺ｫ蝗櫁ｻ｢縺輔○繧玖ｧ貞ｺｦ・亥ｺｦ謨ｰ豕包ｼ・
            constexpr float ANGLE = 50.0f;
            // 蟾ｦ繧定ｦ九ｋ蝣ｴ蜷医・+50蠎ｦ縲∝承繧定ｦ九ｋ蝣ｴ蜷医・-50蠎ｦ //
            float targetAngle = (halfCycle == 0) ? ANGLE : -ANGLE;
            // 鬥悶ｒ蝗櫁ｻ｢縺輔○繧句虚菴懊↓縺九°繧九ヵ繝ｬ繝ｼ繝謨ｰ・郁｣憺俣譛滄俣・・//
            constexpr float CHANGE_DURATION = 15.0f;
            // 鬥悶ｒ蝗櫁ｻ｢縺輔○縺溽憾諷九ｒ菫晄戟縺吶ｋ邨ゆｺ・ヵ繝ｬ繝ｼ繝 //
            constexpr float HOLD_END_FRAME = 27.0f;
            // 鬥悶ｒ蜈・・菴咲ｽｮ縺ｫ謌ｻ縺礼ｵゅｏ繧九ヵ繝ｬ繝ｼ繝 //
            constexpr float RETURN_END_FRAME = 41.0f;

            // 1・壽ｭ｣髱｢縺九ｉ蟾ｦ・医∪縺溘・蜿ｳ・峨∈蝗櫁ｻ｢縺吶ｋ・・-14繝輔Ξ繝ｼ繝・・//
            if (localFrame < CHANGE_DURATION)
            {
                // 鬥悶・蝗櫁ｻ｢陬憺俣險育ｮ礼畑縺ｮ髯､謨ｰ・・HANGE_DURATION - 1・・
                constexpr float CHANGE_DIVISOR = 14.0f;
                float t = localFrame / CHANGE_DIVISOR; // 陬憺俣菫よ焚・・.0 竊・1.0・・
                currentAngle = Lerp(0.0f, targetAngle, t);
            }
            // 2・壼屓霆｢縺励◆迥ｶ諷九ｒ菫晄戟縺吶ｋ・・5-26繝輔Ξ繝ｼ繝・・//
            else if (localFrame < HOLD_END_FRAME)
            {
                currentAngle = targetAngle;
            }
            // 3・壼ｷｦ・医∪縺溘・蜿ｳ・峨°繧画ｭ｣髱｢縺ｸ謌ｻ繧具ｼ・7-40繝輔Ξ繝ｼ繝・・//
            else if (localFrame < RETURN_END_FRAME)
            {
                // 鬥悶ｒ蜈・・菴咲ｽｮ縺ｫ謌ｻ縺怜ｧ九ａ繧九ヵ繝ｬ繝ｼ繝
                constexpr float RETURN_START_FRAME = 27.0f;

                // 鬥悶・蠕ｩ蟶ｰ陬憺俣險育ｮ礼畑縺ｮ髯､謨ｰ・・ETURN_END_FRAME - RETURN_START_FRAME - 1・・
                constexpr float RETURN_DIVISOR = 13.0f;

                float t = (localFrame - RETURN_START_FRAME) / RETURN_DIVISOR; // 陬憺俣菫よ焚・・.0 竊・1.0・・
                currentAngle = Lerp(targetAngle, 0.0f, t);
            }
        }
    }

    // 險育ｮ励＠縺溯ｧ貞ｺｦ繧偵Λ繧ｸ繧｢繝ｳ縺ｫ螟画鋤縺励※繧ｪ繝ｼ繝翫・縺ｫ驕ｩ逕ｨ //
    m_pOwner->SetAngle(currentAngle * DegToRad);
}


////////////////////
// 邨碁℃譎る俣縺九ｉ迴ｾ蝨ｨ縺ｮ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繝輔Ξ繝ｼ繝逡ｪ蜿ｷ繧定ｨ育ｮ励☆繧・
// @return 迴ｾ蝨ｨ縺ｮ繝輔Ξ繝ｼ繝逡ｪ蜿ｷ・域ｵｮ蜍募ｰ乗焚轤ｹ謨ｰ・・
////////////////////
float CIdleHuman::GetCurrentFrame() const
{
    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝輔Ξ繝ｼ繝繝ｬ繝ｼ繝茨ｼ・遘偵≠縺溘ｊ縺ｮ繝輔Ξ繝ｼ繝謨ｰ・・
    constexpr float ANIMATION_FPS = 30.0f;

    // 邨碁℃譎る俣・育ｧ抵ｼ嘉・FPS = 繝輔Ξ繝ｼ繝逡ｪ蜿ｷ //
    return animationTime * ANIMATION_FPS;
}

////////////////////
// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺檎ｵゆｺ・＠縺溘°繧貞愛螳壹☆繧・
// ・育樟蝨ｨ縺ｯ譛ｪ螳溯｣・ｼ・
// @return 邨ゆｺ・愛螳壹・邨先棡
////////////////////
bool CIdleHuman::AnimationFinish() const
{
    if (m_pOwner->GetAnimator()->CurrentFrame() >= 175.0f)
    {
        return true;
    }
    return false;
}

