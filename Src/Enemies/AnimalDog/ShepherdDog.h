#pragma once
#include "Dog.h"

class CSheep;
class CFlock;

class CAShepherdDog : public CADog
{
public:
    CAShepherdDog();
    CAShepherdDog(const VECTOR3& iniPos);
    ~CAShepherdDog();

    void Update() override;
    void ChangeStateHerded(const CSheep* sheep) const;
    void StartHerding();
    void RescueSheep(CSheep* sheep);
    void PopRescueQueue();
    const std::vector<CSheep*>& GetSheeps() const;
    const std::vector<CSheep*>& GetRescueQueue() const;
    void AddSheep(CSheep* sheep);
    bool IsHerding() const { return m_isHerding; }
    void SetFlock(CFlock* flock) { m_pFlock = flock; }
    CFlock* GetFlock() const { return m_pFlock; }

private:
    /// @brief 管理中の羊リストと救出キューから消滅済みの羊を取り除く
    void RemoveDeadSheep();

    /// @brief UFO吸い込み範囲内にいる羊がいるか判定する
    /// @return 吸い込み中かつ吸い込み円内の羊がいる場合true
    bool IsAnySheepBeingSucked() const;

    /// @brief 群れの内側円から外れている羊がいるか判定する
    /// @return 羊が1匹でも群れ外にいる場合true
    bool IsFlockScattered() const;

    std::vector<CSheep*> m_sheeps;
    std::vector<CSheep*> m_rescueQueue; // 救出待ちの羊リスト
    CFlock* m_pFlock = nullptr;
    LerpValue m_lerpValue;
    bool m_isHerding = false;
    bool m_isRescuing = false; // 現在救出中か
};
