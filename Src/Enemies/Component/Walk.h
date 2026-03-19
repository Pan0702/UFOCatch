#pragma once
#include "ComponentBase.h"
#include "../System/AStarPathFinder.h"
class CWalk : public CComponentBase
{
public:
    CWalk(CEnemyBase* e, float speed);
    void Enter() override;
    void Update() override;

private:
    
    /// 境界内に収まるランダムな回転量と移動距離を決定する
    /// ランダムに回転量（-180°～180°）と移動距離（1.0～3.5）を決定し、境界チェックに通るまで最大50回リトライする。
    /// 妥当な組み合わせが見つかった場合、m_turnAmountとm_moveAmountに設定される。
    /// @return 妥当な移動パラメータが見つかった場合true、最大試行回数を超えた場合false
    bool CalcRandomMove();
    
    ///Walkのアニメーションを1.0fの速度で再生する
    void PlayWalkAnimation();
    
    /// 回転角をRadで-π～πの範囲に正規化する
    /// @param angle 回転角
    /// @return -π～πの範囲に正規化した回転角を返す
    static float ClampRotateY(float angle);
    

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
