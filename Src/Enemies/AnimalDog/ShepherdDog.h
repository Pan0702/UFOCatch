#pragma once
#include "Dog.h"

class CSheep;
class CFlog;

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
    void SetFlog(CFlog* flog) { m_pFlog = flog; }
    CFlog* GetFlog() const { return m_pFlog; }

private:
    std::vector<CSheep*> m_sheeps;
    std::vector<CSheep*> m_rescueQueue; // 救出待ちの羊リスト
    CFlog* m_pFlog = nullptr;
    LerpValue m_lerpValue;
    bool m_isHerding = false;
    bool m_isRescuing = false; // 現在救出中か
};
