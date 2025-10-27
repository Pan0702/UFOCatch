#pragma once
#include <chrono>
#include <iostream>

#include "../05_CommonFile/Object3D.h"
#include "AnimalManager.h"


class IACubeState;

class CACube : public CAnimalManager
{
public:
    CACube();
    ~CACube();
    
    void SetState(IACubeState* newState);
    VECTOR3 GetPos() { return transform.position; }
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    void SetRotationY(const float& angle) { transform.rotation.y = angle; }

private:
    void Update() override;
    void Draw();
    template <class C>
    void DrawObject(C c);


    CFbxMesh* m_pRedMesh;
    CFbxMesh* m_pWhiteMesh;
    MeshCollider* m_pRedColl;
    MeshCollider* m_pWhiteColl;
    IACubeState* m_pCurentState;

    VECTOR3 m_maxSize;
    bool m_isInConeArea;
    bool m_pushButton;
    VECTOR3 m_distanceFromObjectToUFO;
    bool m_isMovingToUFO = false;
    bool m_isDestroyMe;
    

};

class CMoveState;
class IACubeState
{
public:
    virtual ~IACubeState() = default;

    virtual void Enter(CACube& cube) = 0; // 状態開始時の初期化
    virtual void Update(CACube& cube) = 0; // 毎フレームの処理
    virtual void Exit(CACube& cube) = 0; // 状態終了時の後処理

    CPlayer* m_pPlayer;
};

class CStopState : public IACubeState
{
public:
    int number;
    void Enter(CACube& cube) override
    {
        number = 0;
    }

    void Update(CACube& cube) override;

    void Exit(CACube& cube) override
    {
        printf("a");
    }
};

class CMoveState : public IACubeState
{
public:
    VECTOR3 BASE_POS = VECTOR3(0, 0, 0);

    float m_moveSpeed;
    float m_turnAmount;
    float m_moveAmount;
    VECTOR3 m_savePos;
    VECTOR3 m_endPos;
    float m_totalPosZMoveAmount;

    void Enter(CACube& cube) override;
    void Update(CACube& cube) override;
    void Exit(CACube& cube) override;
};


class CDestoryState : public IACubeState
{
public:
    void Enter(CACube& cube) override
    {
        printf(" ");
    }
    void Update(CACube& cube) override
    {
        printf(" ");
    }
    void Exit(CACube& cube) override
    {
        printf(" ");
    }
};

class CSuctionState : public IACubeState
{
public:
    VECTOR3 m_distanceFromObjectToUFO;

    void Enter(CACube& cube) override
    {
        MeshCollider* meshColl = new MeshCollider();
        m_pPlayer = new CPlayer();
        m_distanceFromObjectToUFO = m_pPlayer->
            CalcSuctionVelocity(100, cube.GetPos() + VECTOR3(0, meshColl->bBox.max.y, 0));
    };

    void Update(CACube& cube) override
    {
        if (m_pPlayer->GetPos().y <= cube.GetPos().y)
        {
            cube.SetState(new CDestoryState());
        }
        else
        {
            cube.AddPos(m_distanceFromObjectToUFO);
        }
    };
    void Exit(CACube& cube) override
    {
        printf(" ");
    };
};
