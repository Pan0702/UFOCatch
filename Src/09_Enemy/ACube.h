#pragma once
#include <chrono>
#include <iostream>

#include "../05_CommonFile/Object3D.h"
#include "AnimalManager.h"


class InterfaceACubeState;

class CACube : public CAnimalManager
{
public:
    CACube();
    ~CACube();

    void SetState(InterfaceACubeState* newState);
    void Destroy();
    void IsSuctionCheck();
    VECTOR3 SuctionSpeed();

    VECTOR3 GetPos() { return transform.position; }
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    void SetRotationY(const float& angle) { transform.rotation.y = angle; }
    VECTOR3 ObjectMaxSize(){return m_maxSize;}

private:
    void Update() override;
    void Draw();
    template <class C>
    void DrawObject(C c);
    void SetNextState();
 


    CFbxMesh* m_pRedMesh;
    CFbxMesh* m_pWhiteMesh;
    MeshCollider* m_pRedColl;
    MeshCollider* m_pWhiteColl;
    InterfaceACubeState* m_pCurentState;

    VECTOR3 m_maxSize;
    bool m_isInConeArea;
    bool m_pushButton;
    VECTOR3 m_distanceFromObjectToUFO;
};

class CRunState;

//状態を示すインターフェイス//
class InterfaceACubeState
{
public:
    virtual ~InterfaceACubeState() = default;

    virtual void Enter(CACube& cube)
    {
    }

    virtual void Update(CACube& cube)
    {
    }

    virtual void Exit(CACube& cube)
    {
    }

    CPlayer* m_pPlayer;
    
};

class CIdleState : public InterfaceACubeState
{
public:
    void Update(CACube& cube) override;
private:
    int number;
};

class CRunState : public InterfaceACubeState
{
public:
    void Enter(CACube& cube) override;
    void Update(CACube& cube) override;

private:
    VECTOR3 BASE_POS = VECTOR3(0, 0, 0);
    float m_moveSpeed;
    float m_turnAmount;
    float m_moveAmount;
    VECTOR3 m_savePos;
    VECTOR3 m_endPos;
    float m_totalPosZMoveAmount;
};

class CSuctionState : public InterfaceACubeState
{
public:
    VECTOR3 m_distanceFromObjectToUFO;

    void Enter(CACube& cube) override;
    void Update(CACube& cube) override;
};

class CDestoryState : public InterfaceACubeState
{
public:
    void Enter(CACube& cube) override;
};
