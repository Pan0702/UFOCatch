#pragma once
#include <chrono>
#include <iostream>

#include "../05_CommonFile/Object3D.h"
#include "AnimalManager.h"


class IACubeState;

class CACube: public CAnimalManager
{
public:
    CACube();
    ~CACube();
    template<class C>
    void DeletePtr(C c)
    {
      if (c != nullptr)
      {
          delete c;
          c = nullptr;
      }
    }
private:
    void Init();
    void Update() override;
    void Draw();
    void WhiteDraw();
    void RedDraw();
    void EnemyMove();
    void EnemySuction();
    void MoveForUFO(const VECTOR3& animalPos, const VECTOR3& distanceFromObjectToUFO, const int& exp);
    void EnemyDestry();
    void TimerInit();
    void SetState(IACubeState* newState);

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
    float startRotationY;
    int moveAmount;
    
    float time;
    int num;
    bool timeReset;

    
};

class IACubeState {
public:
    virtual ~IACubeState() = default;
    
    virtual void Enter(CACube& cube) = 0; // 状態開始時の初期化
    virtual void Update(CACube& cube) = 0; // 毎フレームの処理
    virtual void Exit(CACube& cube) = 0;  // 状態終了時の後処理
};

class CMoveState : public IACubeState
{
    void Enter(CACube& cube) override {
    }
    void Update(CACube& cube) override {

    }
    void Exit(CACube& cube) override {
    }
};
class CStopState : public IACubeState
{
    void Enter(CACube& cube) override {
    }
    void Update(CACube& cube) override {
        // 例: ここで「ジャンプ」入力があれば JumpingState に遷移
        // if (input.IsJumpPressed()) {
        //     player.SetState(std::make_unique<JumpingState>());
        // }
    }
    void Exit(CACube& cube) override {
    }
};
class CSuctionState : public IACubeState
{
    void Enter(CACube& cube) override {
    }
    void Update(CACube& cube) override {
        // 例: ここで「ジャンプ」入力があれば JumpingState に遷移
        // if (input.IsJumpPressed()) {
        //     player.SetState(std::make_unique<JumpingState>());
        // }
    }
    void Exit(CACube& cube) override {
    }
};
class CDestoryState : public IACubeState
{
public:
    void Enter(CACube& cube) override {
    }
    void Update(CACube& cube) override {
        // 例: ここで「ジャンプ」入力があれば JumpingState に遷移
        // if (input.IsJumpPressed()) {
        //     player.SetState(std::make_unique<JumpingState>());
        // }
    }
    void Exit(CACube& cube) override {
    }
};

