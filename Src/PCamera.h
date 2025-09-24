#pragma once
#include "Object3D.h"

class CPlayerCamera:public Object3D
{
public:
    CPlayerCamera();
    ~CPlayerCamera();
    void PosSet(const VECTOR3& pos);
private:
    void Update() override;

    
    VECTOR3 m_camPos;
    VECTOR3 m_camLook;
    VECTOR3 m_upDir;
};
