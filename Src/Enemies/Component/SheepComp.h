#pragma once
#include "ComponentBase.h"
#include "../AnimalSheep/Sheep.h"

/// <summary>敵AIで使う Herded の情報と処理をまとめる型</summary>
class CHerded : public CComponentBase
{
public:
    /// CHerded を初期化する
    /// @param sheep sheep に渡す値
    CHerded(CSheep* sheep);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Deep Inside Flock を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsDeepInsideFlock() const;

    /// Apply Movement の処理を行う
    /// @param force force に渡す値
    void ApplyMovement(const VECTOR3& force);

    /// Boids を計算する
    /// @return 3次元ベクトル
    VECTOR3 CalcBoids() const;

    /// Escape From Dog を計算する
    /// @return 3次元ベクトル
    VECTOR3 CalcEscapeFromDog() const;

    /// Wandering を計算する
    /// @return 3次元ベクトル
    VECTOR3 CalcWandering(); // ランダムな徘徊行動

    /// Retrun To Flock を計算する
    /// @return 3次元ベクトル
    VECTOR3 CalcRetrunToFlock();


    CSheep* m_pOwner;

    // Wandering用のパラメータ
    VECTOR3 m_wanderTarget;
    float m_wanderTimer;

    // 移動時間管理（一定時間後にIDLEに戻る）
    float m_walkDuration;
    float m_walkTimer;

    // 回転の滑らかさ
    float m_currentRotation;
};


/// <summary>敵AIで使う Panic の情報と処理をまとめる型</summary>
class CPanic : public CComponentBase
{
public:
    /// CPanic を初期化する
    /// @param sheep sheep に渡す値
    CPanic(CSheep* sheep);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Direction を毎フレームの状態を更新する
    void UpdateDirection();

    /// Movement を毎フレームの状態を更新する
    void UpdateMovement() const;

    /// Boundary And Transition を確認する
    void CheckBoundaryAndTransition();

    CSheep* m_pOwner;
    VECTOR3 m_panicDirection; // ランダムな逃げる方向
    float m_changeDirectionTimer = 0.0f;
    float m_panicDuration = 0.0f;
    float m_panicTimer = 0.0f;
    VECTOR3 m_startPos;
};
