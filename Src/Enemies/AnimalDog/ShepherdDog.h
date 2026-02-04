#pragma once
#include "Dog.h"

class CSheep;
class CAShepherdDog : public CADog
{
public:
    CAShepherdDog();
    ~CAShepherdDog();
    
    void Update() override;
    void ChangeStateHerded(const CSheep* sheep) const;
    void StartHerding();
    void RescueSheep(CSheep* sheep);
    void PopRescueQueue();
    const std::vector<CSheep*>& GetSheeps() const;
    const std::vector<CSheep*>& GetRescueQueue() const;
    void AddSheep(CSheep* sheep) ;
private:
    std::vector<CSheep*> m_sheeps;
    std::vector<CSheep*> m_rescueQueue;  // 救助待ちの羊リスト
    LerpValue m_lerpValue;
    bool m_isHerding = false;
    bool m_isRescuing = false;  // 現在救助中か
};
    