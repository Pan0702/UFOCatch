#pragma once
#include "ComponentBase.h"
#include "../AnimalSheep/Sheep.h"
class CHerded : public CComponentBase
{
public:
    CHerded(CSheep* sheep);
    void Enter() override;
    void Update() override;

private:
    CSheep* m_pOwner;
    VECTOR3 CalculateBoids() const;
    VECTOR3 CalculateEscapeFromDog() const;
    VECTOR3 CalculateBoundaryForce() const;  // 荳ｭ蠢・せ縺ｸ縺ｮ蠑輔″蟇・○縺ｨ蜊雁ｾ・宛邏・
    VECTOR3 CalculateWandering();  // 繝ｩ繝ｳ繝繝縺ｪ蠕伜ｾ願｡悟虚

    // Wandering逕ｨ縺ｮ繝代Λ繝｡繝ｼ繧ｿ
    VECTOR3 m_wanderTarget;
    float m_wanderTimer;

    // 遘ｻ蜍墓凾髢鍋ｮ｡逅・ｼ井ｸ螳壽凾髢灘ｾ後↓IDLE縺ｫ謌ｻ繧具ｼ・
    float m_walkDuration;
    float m_walkTimer;

    // 蝗櫁ｻ｢縺ｮ貊代ｉ縺九＆
    float m_currentRotation;
};


class CPanic : public CComponentBase
{
public:
    CPanic(CSheep* sheep);
    void Enter() override;
    void Update() override;

private:
    CSheep* m_pOwner;
    VECTOR3 m_panicDirection;  // 繝ｩ繝ｳ繝繝縺ｪ騾・￡繧区婿蜷・
    float m_changeDirectionTimer = 0.0f;

};