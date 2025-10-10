#pragma once
#include "../Object3D.h"

class CPlayer:public Object3D
{
public:
    CPlayer();
    ~CPlayer();

    VECTOR3 GetPos(){return transform.position;}
    bool IsInConeArea(const VECTOR3& pos) const;
    void DrawCircle(const VECTOR3& center, float radius, DWORD color);
    VECTOR3 SuckUpAnimal(const int& dividend, const VECTOR3& animalPos) const;
    
    void AddExp(int exp){ m_exp += exp; }
    bool GetIsSuckUp() const { return m_isSuckUp; }

private:
    void Update() override;
    void Draw();
    void PlayerMove();
    void CheckLevel();
    void IncreaseConeVertexHeight();
    void CameraPos();


    float m_coneRadius;
    float m_coneDegree;
    int m_suckUpAnimalNum;
    float baseRadius;
    float baseHeight;
    int m_allExp;
    int m_exp;
    bool m_isSuckUp;
    const float ADD_HIGH = 3;

};
