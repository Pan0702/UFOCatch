#pragma once
#include "../05_CommonFile/Object3D.h"

class CPlayer:public Object3D
{
public:
    CPlayer();
    ~CPlayer();
    VECTOR3 GetPos(){return transform.position;}
    ///
    /// オブジェクトの場所を渡し、それがコーンの範囲内ならtrue,外ならfalse
    /// @param targetPos 
    /// @return bool
    /// 
    bool IsWithSuctionCone(const VECTOR3& targetPos) const;



    /// g引き寄せるための移動量を計算 
    /// @param moveTimeSecond
    /// @param animalPos 
    /// @return 1回当たりの移動量を返す
    /// 
    VECTOR3 CalcSuctionDisplacement(const float& moveTimeSecond, const VECTOR3& animalPos) const;

    /// 
    /// @param humanRotateY Radでの度数
    /// @param targetPosition オブジェクトの場所
    /// @return
    /// 
    bool IsTargetInVidionFan(const float& humanRotateY, const VECTOR3& targetPosition);


    void AddExp(int exp){ m_exp += exp; }
    bool GetIsSuckUp() const { return m_SuctionActive; }
    void SubHp(int hp){ m_hp -= hp; }

private:
    
    void Update() override;
   // void Draw();
    
    //判定円の描画
    
    void DrawCircle(const VECTOR3& center, float radius, DWORD color);
    
    void HandleMovementInput();
    
    ///一定数吸い込んだかチェック
    ///
    void CheckLevel();
    ///プレイヤーの高さを増加
    //////
    void IncreaseSuctionConeHeight();
    
    ///カメラの場所を更新
    ///
    void UpdateCameraPos();
    
    /// 
    /// @param dis 
    /// @return
    /// 
    bool IsBeyondMaxDistance(const float& dis);

    /// 2点を与えてヴェクトルからなす角を求めてHumanの視界の角度より大きいか判別
    /// @param vectorTargetToRayEnd 
    /// @param vectorTargetToPlayer 
    /// @return　角度がHumanの視界より小さかったらtrue,大きかったらfalse
    /// 
    bool IsBeyondInsideFanShapeAngle(const VECTOR2& vectorTargetToRayEnd, const VECTOR2& vectorTargetToPlayer);



    float m_coneRadius;
    float m_coneDegree;
    int m_allExp;
    int m_exp;
    int m_hp;
    bool m_SuctionActive;
    


};
