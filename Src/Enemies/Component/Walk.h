#pragma once
#include "ComponentBase.h"

class CWalk : public CComponentBase
{
public:
    CWalk(CEnemyBase* e);
    ~CWalk();
    void Enter() override;
    void Update() override;
    void Exit() override;
    
private:
    //0から360度の間にクランプ
    float ClampRotateY(float angle);
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    
    VECTOR3 m_position;
    float m_moveAmount;
    int m_totalPosZMoveAmount;
    float m_turnAmount;
    float m_currentRotation;
    float m_targetRotation;
    bool m_rotation;
};
