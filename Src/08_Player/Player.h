#pragma once
#include "../05_CommonFile/Object3D.h"

class CPlayer:public Object3D
{
public:
    CPlayer();
    ~CPlayer();

    VECTOR3 GetPos(){return transform.position;}
    ///
    /// オブジェクトのポジションを渡し、それがコーンの範囲内ならtrue、外ならfalseを返す
    /// @param targetPos 
    /// @return bool
    /// 
    bool IsWithSuctionCone(const VECTOR3& targetPos) const;

    ///判定円の描画
    void DrawCircle(const VECTOR3& center, float radius, DWORD color);

    /// 引き寄せるための移動量を計算
    /// @param dividend 
    /// @param animalPos 
    /// @return 1回当たりの移動量を返す
    /// 
    VECTOR3 CalcSuctionVelocity(const int& dividend, const VECTOR3& animalPos) const;

    /// 
    /// @param humanRotateY Radでの度数
    /// @param targetPosition オブジェクトのポジション
    /// @return 
    bool IsTargetInVidionFan(const float& humanRotateY, const VECTOR3& targetPosition);
    bool IsBeyondMaxDistance(const float& dis);
    bool IsBeyondInsideFanShapeAngle(const VECTOR2& vectorA, const VECTOR2& vectorB);

    void AddExp(int exp){ m_exp += exp; }
    bool GetIsSuckUp() const { return m_isSuctionActive; }

private:
    void Update() override;
    void Draw();
    void HandleMovementInput();
    void CheckLevel();
    void IncreaseSuctionConeHeight();
    void UpdateCameraPos();



    float m_coneRadius;
    float m_coneDegree;
    int m_allExp;
    int m_exp;
    bool m_isSuctionActive;


};
