#pragma once
#include "ComponentBase.h"

class CWalk : public CComponentBase
{
public:
    CWalk(CEnemyBase* e);
    CWalk(CEnemyBase* e, float speed);
    void Enter() override;
    void Update() override;
    void Exit() override;
    
private:
    //0から360度の間にクランプ
    float ClampRotateY(float angle);
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    
    VECTOR3 m_position;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    float m_turnAmount;
    float m_currentRotation;
    float m_targetRotation;
    float m_moveSpeed;
    bool m_rotation;
};
