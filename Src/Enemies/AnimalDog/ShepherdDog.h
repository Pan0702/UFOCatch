#pragma once
#include "Dog.h"

/// <summary>敵AIで使う Sheep の情報と処理をまとめる型</summary>
class CSheep;
/// <summary>敵AIで使う Flock の情報と処理をまとめる型</summary>
class CFlock;

/// <summary>敵AIで使う AShepherd Dog の情報と処理をまとめる型</summary>
class CAShepherdDog : public CADog
{
public:
    /// CAShepherdDog を初期化する
    CAShepherdDog();
    /// CAShepherdDog を初期化する
    /// @param iniPos 初期座標
    CAShepherdDog(const VECTOR3& iniPos);
    /// CAShepherdDog の終了処理を行う
    ~CAShepherdDog();

    /// 毎フレームの状態を更新する
    void Update() override;
    /// State Herded を切り替える
    /// @param sheep sheep に渡す値
    void ChangeStateHerded(const CSheep* sheep) const;
    /// Herding を開始する
    void StartHerding();
    /// Rescue Sheep の処理を行う
    /// @param sheep sheep に渡す値
    void RescueSheep(CSheep* sheep);
    /// Rescue Queue を取り出す
    void PopRescueQueue();
    /// Sheeps を取得する
    /// @return 取得した要素一覧
    const std::vector<CSheep*>& GetSheeps() const;
    /// Rescue Queue を取得する
    /// @return 取得した要素一覧
    const std::vector<CSheep*>& GetRescueQueue() const;
    /// Sheep を追加する
    /// @param sheep sheep に渡す値
    void AddSheep(CSheep* sheep);
    /// Herding を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsHerding() const { return m_isHerding; }
    /// Flock を設定する
    /// @param flock flock に渡す値
    void SetFlock(CFlock* flock) { m_pFlock = flock; }
    /// Flock を取得する
    /// @return 対象のポインタ
    CFlock* GetFlock() const { return m_pFlock; }

private:
    /// Dead Sheep を削除する
    void RemoveDeadSheep();

    /// Any Sheep Being Sucked を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsAnySheepBeingSucked() const;

    /// Flock Scattered を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsFlockScattered() const;

    std::vector<CSheep*> m_sheeps;
    std::vector<CSheep*> m_rescueQueue; // 救出待ちの羊リスト
    CFlock* m_pFlock = nullptr;
    LerpValue m_lerpValue;
    bool m_isHerding = false;
    bool m_isRescuing = false; // 現在救出中か
};
