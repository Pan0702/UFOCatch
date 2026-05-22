#pragma once
#include "ComponentBase.h"
#include "../System/AStarPathFinder.h"
/// <summary>敵AIで使う Sheep の情報と処理をまとめる型</summary>
class CSheep;


/// <summary>敵AIで使う Walk の情報と処理をまとめる型</summary>
class CWalk : public CComponentBase
{
public:
    /// CWalk を初期化する
    /// @param e e に渡す値
    /// @param speed speed に渡す値
    CWalk(CEnemyBase* e, float speed);
    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Random Move を計算する
    /// @return 成功または条件を満たす場合 true
    bool CalcRandomMove();

    /// Sheep Random Move を計算する
    /// @param sheep sheep に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CalcSheepRandomMove(const CSheep* sheep);

    /// Default Random Move を計算する
    /// @return 成功または条件を満たす場合 true
    bool CalcDefaultRandomMove();

    /// Walk Animation を再生する
    void PlayWalkAnimation() const;

    /// Clamp Rotate Y を返す
    /// @param angle 角度
    /// @return 計算結果の値
    static float ClampRotateY(float angle);

    /// Adjust Target To Free Cell の処理を行う
    /// @param pTree pTree に渡す値
    /// @param mapBounds mapBounds に渡す値
    /// @param cellSize サイズ
    void AdjustTargetToFreeCell(const CStageQuadTree* pTree,
                                const VECTOR4& mapBounds, float cellSize);

    /// Rotation Only を毎フレームの状態を更新する
    void UpdateRotationOnly();

    /// Blocked By Flock Boundary を判定する
    /// @param moveVec 移動量
    /// @return 成功または条件を満たす場合 true
    bool IsBlockedByFlockBoundary(const VECTOR3& moveVec) const;

    /// Advance Path Index の処理を行う
    void AdvancePathIndex();

    VECTOR3 m_position;
    VECTOR3 m_targetPos;
    float m_turnAmount;
    float m_currentRotation;
    float m_targetRotation;
    float m_moveSpeed;
    bool m_rotation;
    static constexpr float TURN_ANGLE_DEG = 180.0f; // ランダム回転の範囲（度）

    //A*の経路
    std::vector<VECTOR2> m_path;
    //今どのウェイポイントに向かってるか
    int m_pathIndex;
    //A*
    CAStarPathFinder m_pathFinder;
};
