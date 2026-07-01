#pragma once
#include "../Common/Object3D.h"

// プレイヤーのHP及び発見状態を管理するクラス //
class CPlayerHP : public Object3D
{
public:
    // HPの初期値を指定してインスタンスを生成する
    // @param hp 初期HP //
    /// CPlayerHP を初期化する
    /// @param hp hp に渡す値
    CPlayerHP(const int& hp);
    /// CPlayerHP の終了処理を行う
    ~CPlayerHP();
    /// HP を減算する
    void SubHP();
    /// Flag を初期状態に戻す
    void ResetFlag();
    /// HP を取得する
    /// @return 処理結果の数値
    int GetHP() const { return m_currentHp; }
    /// Max HP を取得する
    /// @return 処理結果の数値
    int GetMaxHP() const { return m_maxHp; }
    /// Found Flag を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetFoundFlag() const { return m_found; }
    /// Find Count を取得する
    /// @return 計算結果の値
    float GetFindCount() const { return m_findCount; }
    /// Max Find Count を取得する
    /// @return 計算結果の値
    float GetMaxFindCount() const { return m_findMaxCount; }
    /// Invincible を取得する
    /// @return 計算結果の値
    float GetInvincible() const { return m_invincible; }

private:
    /// 毎フレームの状態を更新する
    void Update() override;

    int m_currentHp;
    int m_maxHp;
    bool m_found;
    bool m_seemToFind;
    float m_invincible;
    float m_findCount;
    float m_findMaxCount;
};
