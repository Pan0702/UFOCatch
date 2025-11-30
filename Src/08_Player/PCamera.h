#pragma once
#include "../05_CommonFile/Object3D.h"
#include "../06_GameLib/LerpValue.h"

class CPlayerCamera:public Object3D
{
public:
    CPlayerCamera();
    ~CPlayerCamera();
    void PosSet(const VECTOR3& pos, const float& distance);
    void ZoomIn(const VECTOR3& pos);
    void ZoomOut(const VECTOR3& pos);
private:
    void Update() override;
    void UpdateCameraLerp();

    
    VECTOR3 m_camPos;
    VECTOR3 m_camLook;
    LerpValueVec3 m_camPosLerp;
    LerpValueVec3 m_camLookLerp;
    
};
