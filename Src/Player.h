#pragma once
#include "Object3D.h"

class CPlayer:public Object3D
{
public:
    CPlayer();
    ~CPlayer();

    VECTOR3 GetPos(){return transform.position;}
private:
    void Update() override;
    void Draw();
    void PlayerMove();
    VECTOR3 GetModelCenter() const;

    CFbxMesh* m_pMesh;
    Animator* m_pAnimator;
};
