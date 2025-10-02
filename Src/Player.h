#pragma once
#include "Object3D.h"

class CPlayer:public Object3D
{
public:
    CPlayer();
    ~CPlayer();

    VECTOR3 GetPos(){return transform.position;}
    bool IsInConeArea(const VECTOR3& pos);
    void DrawCircle(const VECTOR3& center, float radius, DWORD color);
    float SuckUpAnimal(const VECTOR3& pos);

private:
    void Update() override;
    void Draw();
    void PlayerMove();
    float DistnceMillliseconds();


    
    CFbxMesh* m_pMesh;
    Animator* m_pAnimator;

    float m_coneRadius;
    float m_coneDegree;
};
