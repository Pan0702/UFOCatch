#pragma once
#include "../05_CommonFile/Object3D.h"

class CPlayerHP : public Object3D
{
public:
    CPlayerHP(const int& hp);
    ~CPlayerHP();
    void SubHP();
    void ResetFlag();
    
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
