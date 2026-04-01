#include "TutorialDisplayInfo.h"

namespace
{
    // 遘ｻ蜍墓桃菴弑I縺ｮ謠冗判菴咲ｽｮ繝ｻ繧ｵ繧､繧ｺ //
    constexpr int MOVE_UI_X = 486;
    constexpr int MOVE_UI_Y = 580;
    constexpr int MOVE_UI_SRC_X = 0;
    constexpr int MOVE_UI_SRC_Y = 0;
    constexpr int MOVE_UI_WIDTH = 382;
    constexpr int MOVE_UI_HEIGHT = 124;

    // 蜷ｸ縺・ｾｼ縺ｿ謫堺ｽ弑I縺ｮ謠冗判菴咲ｽｮ繝ｻ繧ｵ繧､繧ｺ //
    constexpr int SUCTION_UI_X = 477;
    constexpr int SUCTION_UI_Y = 600;
    constexpr int SUCTION_UI_SRC_X = 0;
    constexpr int SUCTION_UI_SRC_Y = 125;
    constexpr int SUCTION_UI_WIDTH = 406;
    constexpr int SUCTION_UI_HEIGHT = 84;

    // 逶ｮ讓呵｡ｨ遉ｺUI縺ｮ謠冗判菴咲ｽｮ繝ｻ繧ｵ繧､繧ｺ //
    constexpr int TARGET_UI_X = 330;
    constexpr int TARGET_UI_Y = 30;
    constexpr int TARGET_UI_SRC_X = 0;
    constexpr int TARGET_UI_BASE_SRC_Y = 209;
    constexpr int TARGET_UI_QUEST_HEIGHT = 70;
    constexpr int TARGET_UI_WIDTH = 700;
}

CTutorialDisplayInfo::CTutorialDisplayInfo()
{
    m_pImage = std::make_unique<CSpriteImage>("data/Tutorial/TutorialUI.png");
    m_nDisplayType = 0;
    m_questNum = 0;
}

CTutorialDisplayInfo::~CTutorialDisplayInfo()
{
}

void CTutorialDisplayInfo::Draw()
{
    OperationDraw();
    TargetDraw();
}

////////////////////
// 謫堺ｽ懆ｪｬ譏散I繧呈緒逕ｻ縺吶ｋ 
////////////////////
void CTutorialDisplayInfo::OperationDraw() const
{
    CSprite spr;
    switch (m_nDisplayType)
    {
    case Move:
        spr.Draw(m_pImage.get(), MOVE_UI_X, MOVE_UI_Y, MOVE_UI_SRC_X, MOVE_UI_SRC_Y, MOVE_UI_WIDTH, MOVE_UI_HEIGHT);
        break;
    case Suction:
        spr.Draw(m_pImage.get(), SUCTION_UI_X, SUCTION_UI_Y, SUCTION_UI_SRC_X, SUCTION_UI_SRC_Y, SUCTION_UI_WIDTH, SUCTION_UI_HEIGHT);
        break;
    default:
        break;
    }
}

////////////////////
// 逶ｮ讓呵｡ｨ遉ｺUI繧呈緒逕ｻ縺吶ｋ 
////////////////////
void CTutorialDisplayInfo::TargetDraw() const
{
    CSprite spr;
    int srcY = TARGET_UI_BASE_SRC_Y + TARGET_UI_QUEST_HEIGHT * m_questNum;
    spr.Draw(m_pImage.get(), TARGET_UI_X, TARGET_UI_Y, TARGET_UI_SRC_X, srcY, TARGET_UI_WIDTH, TARGET_UI_QUEST_HEIGHT);
}

