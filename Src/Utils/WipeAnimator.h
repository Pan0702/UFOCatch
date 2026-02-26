#pragma once
#include "Sprite3D.h"
#include <vector>

/// <summary>
/// ボタン切り替え時の上下ワイプアニメーションを管理するクラス
/// </summary>
class WipeAnimator
{
public:
    WipeAnimator(int slotCount)
        : m_clipT(slotCount, 0.0f), m_closing(slotCount, true)
    {
        if (slotCount > 0)
        {
            m_clipT[0]   = 1.0f;
            m_closing[0] = false;
        }
    }

    // 選択変更時に呼ぶ (direction: 1=下移動, -1=上移動)
    void ChangeTo(int prevIndex, int newIndex, int direction)
    {
        m_direction = direction;
        m_clipT[prevIndex]   = 1.0f;
        m_closing[prevIndex] = true;
        m_clipT[newIndex]    = 0.0f;
        m_closing[newIndex]  = false;
    }

    // 毎フレーム呼ぶ
    void Update(float speed = 0.12f, float deltaTime = 0.0f)
    {
        for (int i = 0; i < (int)m_clipT.size(); i++)
        {
            if (m_closing[i])
            {
                m_clipT[i] -= speed * deltaTime;
                if (m_clipT[i] < 0.0f) m_clipT[i] = 0.0f;
            }
            else
            {
                m_clipT[i] += speed * deltaTime;
                if (m_clipT[i] > 1.0f) m_clipT[i] = 1.0f;
            }
        }
    }

    // スロットindexをワイプ付きで描画する
    void Draw(CSprite& spr, int index, CSpriteImage* image,
              float posX, float posY, float srcW, float srcH)
    {
        float t = m_clipT[index];
        if (t <= 0.0f) return;

        float clipH = srcH * t;
        float drawY = posY;
        float srcY  = 0.0f;

        // 下端固定(上からワイプ): 下移動の閉じる / 上移動の開く
        bool useBottomFixed = (m_direction == 1  &&  m_closing[index])
                           || (m_direction == -1 && !m_closing[index]);
        if (useBottomFixed)
        {
            drawY = posY + (srcH - clipH);
            srcY  = srcH - clipH;
        }

        spr.Draw(image, posX, drawY, 0, (DWORD)srcY, (DWORD)srcW, (DWORD)clipH);
    }

private:
    int m_direction = 1;
    std::vector<float> m_clipT;
    std::vector<bool>  m_closing;
};