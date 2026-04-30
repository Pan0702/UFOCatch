#pragma once
#include "ComponentBase.h"
#include "../System/AStarPathFinder.h"
class CSheep;


class CWalk : public CComponentBase
{
public:
    CWalk(CEnemyBase* e, float speed);
    void Enter() override;
    void Update() override;

private:
    /// @brief 境界内に収まるランダムな回転量と移動距離を決定する
    /// @brief ランダムに回転量（-180°～180°）と移動距離（1.5～4.0）を決定し、境界チェックに通るまで最大50回リトライする。
    /// @brief 妥当な組み合わせが見つかった場合、m_turnAmountとm_targetPosに設定される。
    /// @return 妥当な移動パラメータが見つかった場合true、最大試行回数を超えた場合false
    bool CalcRandomMove();

    /// @brief 羊用のランダム移動目標を群れの内側または外側リングから決定する
    /// @param sheep 群れ情報を参照する羊
    /// @return 目標位置と回転量を決定できた場合true
    bool CalcSheepRandomMove(const CSheep* sheep);

    /// @brief 通常の敵用に境界内へ収まるランダムな移動目標を決定する
    /// @return 境界内の移動目標が見つかった場合true、最大試行回数を超えた場合false
    bool CalcDefaultRandomMove();

    /// @brief Walkのアニメーションを1.0fの速度で再生する
    void PlayWalkAnimation() const;

    /// @brief 回転角をRadで-π～πの範囲に正規化する
    /// @param angle 回転角
    /// @return -π～πの範囲に正規化した回転角を返す
    static float ClampRotateY(float angle);

    /// @brief m_targetPos がステージオブジェクトと重なっていれば、近隣の空きセルに修正する
    /// @param pTree      : ステージ四分木（nullptr なら何もしない）
    /// @param mapBounds  : CGameInstance::Get()->GetMapSize() の値(xMin, zMin, xMax, zMax)
    /// @param cellSize   :グリッドセルサイズ（m_pathFinder.GetCellSize()）
    void AdjustTargetToFreeCell(const CStageQuadTree* pTree,
                                const VECTOR4& mapBounds, float cellSize);

    /// @brief 経路が無いとき、回転のみを補間して終了判定する
    void UpdateRotationOnly();

    /// @brief 羊が群れの移動可能エリアの外に出ようとしていないか
    bool IsBlockedByFlockBoundary(const VECTOR3& moveVec) const;

    /// @brief 経路インデックスを進める。直線的に並ぶ次ポイントはスキップ
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
