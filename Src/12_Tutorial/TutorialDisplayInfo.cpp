#include "TutorialDisplayInfo.h"

namespace
{
    // 移動操作UIの描画位置・サイズ //
    constexpr int MOVE_UI_X = 486;
    constexpr int MOVE_UI_Y = 580;
    constexpr int MOVE_UI_SRC_X = 0;
    constexpr int MOVE_UI_SRC_Y = 0;
    constexpr int MOVE_UI_WIDTH = 382;
    constexpr int MOVE_UI_HEIGHT = 124;

    // 吸い込み操作UIの描画位置・サイズ //
    constexpr int SUCTION_UI_X = 477;
    constexpr int SUCTION_UI_Y = 600;
    constexpr int SUCTION_UI_SRC_X = 0;
    constexpr int SUCTION_UI_SRC_Y = 125;
    constexpr int SUCTION_UI_WIDTH = 406;
    constexpr int SUCTION_UI_HEIGHT = 84;

    // 目標表示UIの描画位置・サイズ //
    constexpr int TARGET_UI_X = 330;
    constexpr int TARGET_UI_Y = 30;
    constexpr int TARGET_UI_SRC_X = 0;
    constexpr int TARGET_UI_BASE_SRC_Y = 209;
    constexpr int TARGET_UI_QUEST_HEIGHT = 70;
    constexpr int TARGET_UI_WIDTH = 700;
}

CTutorialDisplayInfo::CTutorialDisplayInfo()
{
    m_pSprite = new CSprite();
    m_pImage = new CSpriteImage("data/Tutorial/TutorialUI.png");
    m_nDisplayType = 0;
    m_questNum = 0;
}

CTutorialDisplayInfo::~CTutorialDisplayInfo()
{
    SAFE_DELETE(m_pSprite);
    SAFE_DELETE(m_pImage);
}

void CTutorialDisplayInfo::Draw()
{
    OperationDraw();
    TargetDraw();
}

////////////////////
// 操作説明UIを描画する //
////////////////////
void CTutorialDisplayInfo::OperationDraw() const
{
    switch (m_nDisplayType)
    {
    case Move:
        m_pSprite->Draw(m_pImage, MOVE_UI_X, MOVE_UI_Y, MOVE_UI_SRC_X, MOVE_UI_SRC_Y, MOVE_UI_WIDTH, MOVE_UI_HEIGHT);
        break;
    case Suction:
        m_pSprite->Draw(m_pImage, SUCTION_UI_X, SUCTION_UI_Y, SUCTION_UI_SRC_X, SUCTION_UI_SRC_Y, SUCTION_UI_WIDTH, SUCTION_UI_HEIGHT);
        break;
    default:
        break;
    }
}

////////////////////
// 目標表示UIを描画する //
////////////////////
void CTutorialDisplayInfo::TargetDraw()
{
    int srcY = TARGET_UI_BASE_SRC_Y + TARGET_UI_QUEST_HEIGHT * m_questNum;
    m_pSprite->Draw(m_pImage, TARGET_UI_X, TARGET_UI_Y, TARGET_UI_SRC_X, srcY, TARGET_UI_WIDTH, TARGET_UI_QUEST_HEIGHT);
}
