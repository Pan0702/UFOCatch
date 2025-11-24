#include "PHP.h"

CPlayerHP::CPlayerHP(const int& hp)
    : m_maxHp(hp)
{
    m_currentHp = hp;
    m_pSprite = new CSprite();
    m_pHpImage = new CSpriteImage();
    m_pHpDamageImage = new CSpriteImage();
    m_pHpImage->Load("data/Player/hp.png");
    m_pHpDamageImage->Load("data/Player/damageHp.png");
    m_hpPos = VECTOR2(50, 30);
    m_imageSize = VECTOR2(256, 256);
    m_drawImageSize = VECTOR2(128,128);
    m_k =  10 + m_drawImageSize.x;
}

CPlayerHP::~CPlayerHP()
{
    if (m_pHpImage != nullptr)
    {
        SAFE_DELETE(m_pHpImage);
        m_pHpImage = nullptr;
    }
    if (m_pHpDamageImage != nullptr)
    {
        SAFE_DELETE(m_pHpDamageImage);
        m_pHpDamageImage = nullptr;
    }
    if (m_pSprite)
    {
        SAFE_DELETE(m_pSprite);
        m_pSprite = nullptr;
    }
}


void CPlayerHP::Update()
{
    if (m_currentHp <= 0)
    {
    }
}

void CPlayerHP::Draw()
{
    for (int i = 0; i < m_maxHp; i++)
    {
        if (i <= m_currentHp - 1)
        {
            m_pSprite->Draw(m_pHpImage,m_hpPos.x + i * m_k,m_hpPos.y,0,0,m_imageSize.x,m_imageSize.y,m_drawImageSize.x,m_drawImageSize.y);
        }
        else
        {
            m_pSprite->Draw(m_pHpDamageImage,m_hpPos.x + i * m_k,m_hpPos.y,0,0,m_imageSize.x,m_imageSize.y,m_drawImageSize.x,m_drawImageSize.y);
        }
    }
}
