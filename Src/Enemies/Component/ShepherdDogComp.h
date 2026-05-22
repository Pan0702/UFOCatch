#pragma once
#include "ComponentBase.h"
#include "Destroy.h"

#include "../System/AStarPathFinder.h"
/// <summary>敵AIで使う AShepherd Dog の情報と処理をまとめる型</summary>
class CAShepherdDog;
/// <summary>敵AIで使う Sheep の情報と処理をまとめる型</summary>
class CSheep;

/// <summary>敵AIで使う Shepherd Dog Walk の情報と処理をまとめる型</summary>
class CShepherdDogWalk : public CComponentBase
{
public:
    /// CShepherdDogWalk を初期化する
    /// @param dog dog に渡す値
    /// @param speed speed に渡す値
    CShepherdDogWalk(CAShepherdDog* dog, float speed);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Random Move In Ring を計算する
    /// @return 成功または条件を満たす場合 true
    bool CalcRandomMoveInRing();

    /// Move To を実行できるか判定する
    /// @param nextPos 座標
    /// @return 成功または条件を満たす場合 true
    bool CanMoveTo(const VECTOR3& nextPos) const;

    /// Inside Inner Circle を判定する
    /// @param nextPos 座標
    /// @return 成功または条件を満たす場合 true
    bool IsInsideInnerCircle(const VECTOR3& nextPos) const;

    /// Try Rotate Toward を返す
    /// @param targetPos 座標
    /// @return 成功または条件を満たす場合 true
    bool TryRotateToward(const VECTOR3& targetPos);

    /// Slide Along Inner Circle を返す
    /// @param moveVec 移動量
    /// @return 3次元ベクトル
    VECTOR3 SlideAlongInnerCircle(const VECTOR3& moveVec) const;

    CAShepherdDog* m_pOwner = nullptr;
    VECTOR3 m_targetPos = {};
    float m_moveSpeed = 0.0f;
};

/// <summary>敵AIで使う Collecting の情報と処理をまとめる型</summary>
class CCollecting : public CComponentBase
{
public:
    /// CCollecting を初期化する
    /// @param dog dog に渡す値
    /// @param speed speed に渡す値
    CCollecting(CAShepherdDog* dog, float speed);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Advance Path Index の処理を行う
    void AdvancePathIndex();

    /// Recompute Path の処理を行う
    void RecomputePath();

    /// Furthest Stray Sheep を検索する
    /// @return 対象のポインタ
    CSheep* FindFurthestStraySheep() const;

    /// Collect Target Pos を計算する
    /// @param targetSheep 対象
    /// @return 成功または条件を満たす場合 true
    bool CalcCollectTargetPos(CSheep* targetSheep);

    /// Build Path の処理を行う
    void BuildPath();

    /// Finish And Herd Target の処理を行う
    void FinishAndHerdTarget();

    CAShepherdDog* m_pOwner;
    VECTOR3 m_targetPos = {0, 0, 0};
    float m_moveSpeed;

    std::vector<VECTOR2> m_path;
    int m_pathIndex = 0;
    CAStarPathFinder m_pathFinder;
    float m_repathTimer = 0.0f;
    CSheep* m_targetSheep = nullptr;
    const float REPATH_INTERVAL = 0.2f;
};

/// <summary>敵AIで使う Driving の情報と処理をまとめる型</summary>
class CDriving : public CComponentBase
{
public:
    /// CDriving を初期化する
    /// @param dog dog に渡す値
    /// @param speed speed に渡す値
    CDriving(CAShepherdDog* dog, float speed);

    /// Enter の処理を行う
    void Enter() override;

    /// 毎フレームの状態を更新する
    void Update() override;

private:
    CAShepherdDog* m_pOwner;
    VECTOR3 m_targetPos = {0, 0, 0};
    VECTOR3 m_destination = {0, 0, 0};
    float m_moveSpeed;
};

/// <summary>敵AIで使う Rescue の情報と処理をまとめる型</summary>
class CRescue : public CComponentBase
{
public:
    /// CRescue を初期化する
    /// @param dog dog に渡す値
    CRescue(CAShepherdDog* dog);

    /// Enter の処理を行う
    void Enter() override;

    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// <summary>Phase で扱う状態や種別を表す列挙型</summary>
    enum Phase : int8_t
    {
        APPROACH_SHEEP, // フェーズ1: 羊に近づく
        GUIDE_TO_CENTER // フェーズ2: 重心まで誘導
    };

    CAShepherdDog* m_pOwner;
    CSheep* m_targetSheep = nullptr; // 救出対象の羊
    VECTOR3 m_centroid = {0, 0, 0}; // 群れの重心
    Phase m_phase;
};

/// <summary>敵AIで使う Destroy Shepherd Dog の情報と処理をまとめる型</summary>
class CDestroyShepherdDog : public CDestroy
{
public:
    /// CDestroyShepherdDog を初期化する
    /// @param dog dog に渡す値
    /// @param score score に渡す値
    /// @param exp exp に渡す値
    CDestroyShepherdDog(CAShepherdDog* dog, int score, float exp);
    /// Enter の処理を行う
    void Enter() override;

private:
    CAShepherdDog* m_pDog;
};
