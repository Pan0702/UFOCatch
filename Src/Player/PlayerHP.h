#pragma once
#include "../Common/Object3D.h"
#include "../Scene/Play/PlayUI.h"

// プレイヤーのHP及び発見状態を管理するクラス //
class CPlayerHP : public Object3D
{
public:
    // HPの初期値を指定してインスタンスを生成する
    // @param hp 初期HP //
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
    void Update() override;

    int m_currentHp;
    int m_maxHp;
    bool m_found;
    bool m_seemToFind;
    float m_invincible;
    float m_findCount;
    float m_findMaxCount;
    CPlayUI* m_pUI;
};
