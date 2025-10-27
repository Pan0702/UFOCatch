#pragma once
#include <chrono>
#include <iostream>
#include <queue>

#include <d3d11.h>

#include "../05_CommonFile/Object3D.h"
#include "../08_Player/Player.h"


class InterfaceACubeState;

class CACube : public Object3D
{
public:
    CACube(const VECTOR3& iniPos = VECTOR3(0,0,0),const VECTOR2& moveAreaSize = VECTOR2(10,10));
    ~CACube();

    void SetState(InterfaceACubeState* newState);
    void Destroy();
    void IsSuctionCheck();
    VECTOR3 SuctionSpeed();

    VECTOR3 GetPos() { return transform.position; }
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    void SetRotationY(const float& angle) { transform.rotation.y = angle; }
    VECTOR3 ObjectMaxSize(){return m_maxSize;}
    VECTOR2 MoveAreaSize(){return m_moveAreaSize;}

    std::queue<InterfaceACubeState*> m_actionQueue;

private:
    void Update() override;
    void Draw();
    template <class C>
    void DrawObject(C c);
    void SetNextState();
 

private:
    CFbxMesh* m_pRedMesh;
    CFbxMesh* m_pWhiteMesh;
    MeshCollider* m_pRedColl;
    MeshCollider* m_pWhiteColl;
    InterfaceACubeState* m_pCurentState;
    CPlayer* m_pPlayer;

    VECTOR3 m_maxSize;
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_moveAreaSize;
   
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
    CIdleState() : number(0) {}
    void Update(CACube& cube) override;
private:
    int number;
};

class CRunState : public InterfaceACubeState
{
public:
    CRunState() : m_moveSpeed(0), m_turnAmount(0), m_moveAmount(0), m_totalPosZMoveAmount(0) {}
    void Enter(CACube& cube) override;
    void Update(CACube& cube) override;
    bool boundaryCheck(const VECTOR2& areaSize);

private:
    VECTOR3 BASE_POS = VECTOR3(0, 0, 0);
    float m_moveSpeed;
    float m_turnAmount;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    VECTOR3 m_position;
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
