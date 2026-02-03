#pragma once
#include "Dog.h"
#include "../AnimalSheep/Sheep.h"
#include "../System/Flog.h"

class CAShepherdDog : public CADog
{
public:
    CAShepherdDog();
    ~CAShepherdDog();
    
    void Update() override;
    void ChangeStateHerded(const CSheep* sheep) const;
    void Draw() override;
    void StartHerding();
    void RescueSheep(CSheep* sheep);

    const std::vector<CSheep*>& GetSheeps() const { return m_sheeps; }
    const std::vector<CSheep*>& GetRescueQueue() const { return m_rescueQueue; }
    void PopRescueQueue() { if (!m_rescueQueue.empty()) m_rescueQueue.erase(m_rescueQueue.begin()); }
    void AddSheep(CSheep* sheep) { m_sheeps.push_back(sheep); }
private:
    std::vector<CSheep*> m_sheeps;
    std::vector<CSheep*> m_rescueQueue;  // 救助待ちの羊リスト
    LerpValue m_lerpValue;
    bool m_isHerding = false;
    bool m_isRescuing = false;  // 現在救助中か
};
    