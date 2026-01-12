#include "DisplayInfo.h"

#include "Timer.h"
#include "../08_Player/PHP.h"
#include "../08_Player/Player.h"

CDisplayInfo::CDisplayInfo()
{
    m_pSprite = new CSprite();
    m_playUIImage = new CSpriteImage("data/PlayUI.png");
    m_expImage = new CSpriteImage("data/PlayUIParts.png");
    m_giwakuImage = new CSpriteImage("data/Circle.png");

    // 経験値ゲージ初期化
    m_prevProportion = 0;
    m_currentWidth = 0;

    // 疑惑ゲージ初期化
    m_prevGiwakuProportion = 0;
    m_currentAngle = 0;
    
    SetDrawOrder(-100);
}

CDisplayInfo::~CDisplayInfo()
{
    SAFE_DELETE(m_pSprite);
    SAFE_DELETE(m_playUIImage);
}

void CDisplayInfo::Update()
{
    // Lerp更新
    float nextWidth = m_xpWeightLerp.Update(SceneManager::DeltaTime());

    // Lerp中、または値が更新された場合
    if (m_xpWeightLerp.IsLerping() || nextWidth != m_currentWidth)
    {
        m_currentWidth = nextWidth;

        // 満タンに達したかチェック（レベルアップ演出完了時）
        if (m_currentWidth >= 1224.0f)
        {
            m_currentWidth -= 1224.0f;
            m_prevProportion = 0; // 次のExpDrawで0から余剰分へのLerpを開始させる
            m_xpWeightLerp.ForceSetValue(m_currentWidth);
        }
    }
}

void CDisplayInfo::Draw()
{
    ExpDraw();
    //BaseUIを描画
    m_pSprite->Draw(m_playUIImage, 0, 0, 0, 0, 1366, 768);
    GiwakuDraw();
    TimeDraw();
}


////////////////////
// 疑惑ゲージを描画する //
////////////////////
void CDisplayInfo::GiwakuDraw()
{
    CPlayerHP* pHp = ObjectManager::FindGameObject<CPlayerHP>();
    //割合を計算
    float proportion = avoidZero(pHp->GetFindCount() / pHp->GetMaxFindCount());
    //疑惑ゲージを描画
    m_pSprite->DrawCircle(m_giwakuImage, 1122, 469, 0, 0, 230, 230,0.0f, proportion * XM_2PI);

    if (pHp->GetFoundFlag())
    {
        //疑惑から確信に変わったときの見た目を描画
        m_pSprite->Draw(m_expImage, 1192, 588, 0, 0, 97, 73);
    }
}

////////////////////
// 経験値ゲージを描画する //
////////////////////
void CDisplayInfo::ExpDraw()
{
    //LvBaseを描画
    m_pSprite->Draw(m_expImage, 144, 713, 0, 160, 1224, 55);
    CPlayer* pl = ObjectManager::FindGameObject<CPlayer>();
    if (!pl) return;

    //割合を計算
    float proportion = avoidZero(pl->GetExp() / pl->GetAllExp());

    static constexpr float epsilon = 0.001f;

    // Lerpが終わっていたら新しい目標を設定
    if (!m_xpWeightLerp.IsLerping())
    {
        // レベルアップしたかどうか（割合が減少したか、または1.0を超えたか）
        if (proportion < m_prevProportion - epsilon || proportion >= 1.0f)
        {
            // 満タンまでLerpさせる
            m_xpWeightLerp.Start(m_currentWidth, 1224.0f, 0.5f);
            // m_prevProportionはUpdateのリセット処理で0になるのを待つ
        }
        // 通常の経験値増加
        else if (fabs(proportion - m_prevProportion) > epsilon)
        {
            float targetWidth = 1224.0f * proportion;
            m_xpWeightLerp.Start(m_currentWidth, targetWidth, 0.5f);
            m_prevProportion = proportion;
        }
    }

    //LvBarを描画
    m_pSprite->Draw(m_expImage, 144, 721, 0, 100, m_currentWidth, 47);
}

////////////////////
// タイマーを描画する //
////////////////////
void CDisplayInfo::TimeDraw()
{
    CTimer* pTimer = ObjectManager::FindGameObject<CTimer>();
    if (!pTimer) return;

    int time = static_cast<int>(pTimer->GetTime());

    // 桁数を計算
    int count = 0;
    int tmp = time;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    if (count < 2) count = 2;

    // 各桁を左から順に描画
    for (int i = 0; i < count; i++)
    {
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (time / divisor) % 10;
        m_pSprite->Draw(m_expImage, 230 + i * 58, 640, 68 * num, 540, 68, 103, 55, 67);
    }

}

void CDisplayInfo::HPDraw()
{
}
