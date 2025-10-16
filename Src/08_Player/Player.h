#pragma once
#include "../05_CommonFile/Object3D.h"

class CPlayer:public Object3D
{
public:
    CPlayer();
    ~CPlayer();

    VECTOR3 GetPos(){return transform.position;}
    bool IsWithSuctionCone(const VECTOR3& targetPos) const;
    void DrawCircle(const VECTOR3& center, float radius, DWORD color);
    VECTOR3 CalcSuctionVelocity(const int& dividend, const VECTOR3& animalPos) const;
    bool IsTargetInVidionFan(const float& humanRotateY, const VECTOR3& targetPosition);
    bool IsBeyondMaxDistance(const float& dis);
    bool IsBeyondInsideFanShapeAngle(const VECTOR3& vectorA, const VECTOR3& vectorB);

    void AddExp(int exp){ m_exp += exp; }
    bool GetIsSuckUp() const { return m_isSuctionActive; }

private:
    void Update() override;
    void Draw();
    void HandleMovementInput();
    void CheckLevel();
    void IncreaseSuctionConeHeight();
    void UpdateCameraPos();



    float m_coneRadius;
    float m_coneDegree;
    int m_allExp;
    int m_exp;
    bool m_isSuctionActive;


};
