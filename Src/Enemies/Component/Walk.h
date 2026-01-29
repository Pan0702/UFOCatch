#pragma once
#include "ComponentBase.h"

class CWalk : public CComponentBase
{
public:
    CWalk(CEnemyBase* e, float speed);
    void Enter() override;
    void Update() override;

private:
    
    /// 境界内に収まるランダムな回転量と移動距離を 生成する
    /// ランダムに回転量（-180°～+180°）と移動距離  （1 .0～3.5）を生成し、 境界チェックに通るまで最大50回リトライする。
    /// 有効な組み合わせが見つかった場合、m_turnAm  ountとm_moveAmountに設定される。
    /// @return 有効な移動パラメータが見つかった場  合true、 最大試行回数を超えた場合false
    bool CalcRandomMove();
    
    /*
    ///  回転・移動後の位置が境界内に収まるかチェック
    /// @param areaSize エリアのサイズ
    /// @return 境界内ならtrue、境界外ならfalse
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    */
    ///Walkのアニメーションを1.0fの速度で再生する
    void PlayWalkAnimation();
    
    /// 角度をradで-π～πの間に正規化する
    /// @param angle 角度
    /// @return -π～πの間に正規化した角度を返す
    static float ClampRotateY(float angle);
    

    VECTOR3 m_position;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    float m_turnAmount;
    float m_currentRotation;
    float m_targetRotation;
    float m_moveSpeed;
    bool m_rotation;
    static constexpr float kTurnAngleDeg = 180.0f; // ランダム回転の範囲（度）。
};
