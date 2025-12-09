#include "DisplayInfo.h"

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
    tmp = 0;
}

CDisplayInfo::~CDisplayInfo()
{
    SAFE_DELETE(m_pSprite);
    SAFE_DELETE(m_playUIImage);
}

void CDisplayInfo::Update()
{
    if (m_xpWeightLerp.IsLerping())
    {
        m_currentWidth += (m_xpWeightLerp.Update(SceneManager::DeltaTime()) - m_currentWidth);
        if (m_currentWidth >= 1224.0f)
        {
            m_currentWidth = m_currentWidth - 1224;
            m_prevProportion = 0;
        }
    }

    ImGui::Begin("DisplayInfo");
    ImGui::SliderFloat("tst",&tmp,0,XM_2PI);
    ImGui::End();
}

void CDisplayInfo::Draw()
{
    ExpDraw();
    //BaseUIを描画
    m_pSprite->Draw(m_playUIImage, 0, 0, 0, 0, 1366, 768);
    GiwakuDraw();
}


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

void CDisplayInfo::ExpDraw()
{
    //LvBaseを描画
    m_pSprite->Draw(m_expImage, 144, 713, 0, 160, 1224, 55);
    CPlayer* pl = ObjectManager::FindGameObject<CPlayer>();
    if (!pl) return;
    
    //割合を計算
    float proportion = avoidZero(pl->GetExp() / pl->GetAllExp());
    static constexpr float epsilon = 0.001f;
    
    //前の割合と今の割合の差に変化があって、lerpが終わってたら
    if (fabs(proportion - m_prevProportion) > epsilon && !m_xpWeightLerp.IsLerping())
    {
        float targetWidth = 1224 * (proportion < m_prevProportion ? 1.0f : proportion);
        //Lerpスタート
        m_xpWeightLerp.Start(m_currentWidth, targetWidth, 0.5f);
        //前の割合を更新
        m_prevProportion = proportion;
    }
    //Lvを描画
    m_pSprite->Draw(m_expImage, 144, 721, 0, 100, m_currentWidth, 47);
}
