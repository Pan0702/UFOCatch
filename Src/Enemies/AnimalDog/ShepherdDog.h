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
    void AddSheep(CSheep* sheep);
    bool IsHerding() const { return m_isHerding; }
private:
    std::vector<CSheep*> m_sheeps;
    std::vector<CSheep*> m_rescueQueue;  // 謨大勧蠕・■縺ｮ鄒翫Μ繧ｹ繝・
    LerpValue m_lerpValue;
    bool m_isHerding = false;
    bool m_isRescuing = false;  // 迴ｾ蝨ｨ謨大勧荳ｭ縺・
};
    