#pragma once
#include "../Common/Object3D.h"

// 繝励Ξ繧､繝､繝ｼ縺ｮHP蜿翫・逋ｺ隕狗憾諷九ｒ邂｡逅・☆繧九け繝ｩ繧ｹ //
class CPlayerHP : public Object3D
{
public:
    // HP縺ｮ蛻晄悄蛟､繧呈欠螳壹＠縺ｦ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
    // @param hp 蛻晄悄HP //
    CPlayerHP(const int& hp);
    ~CPlayerHP();
    void SubHP();
    void ResetFlag();
    int GetHP() const { return m_currentHp; }
    int GetMaxHP() const { return m_maxHp; }
    bool GetFoundFlag() const { return m_found; }
    float GetFindCount() const { return m_findCount; }
    float GetMaxFindCount() const { return m_findMaxCount; }
    float GetInvincible() const { return m_invincible; }

private:
    void Update() override;;
    
    int m_currentHp;
    int m_maxHp;
    bool m_found;
    bool m_seemToFind;
    float m_invincible;
    float m_findCount;
    float m_findMaxCount;
};
