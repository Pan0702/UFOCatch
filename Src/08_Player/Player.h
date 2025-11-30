#pragma once
#include "../05_CommonFile/Object3D.h"
#include "../06_GameLib/LerpValue.h"

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
    
    void AddExp(int exp){ m_exp += exp; }
    bool GetIsSuckUp() const { return m_SuctionActive; }

private:
    
    void Update() override;
    void Draw() override;
    
    //判定円の描画
    ///Debug///
    void DrawCircle(const VECTOR3& center, float radius, DWORD color);
    
    void HandleMovementInput();
    
    ///一定数吸い込んだかチェック ///
    void CheckLevel();
    ///プレイヤーの高さを増加 ///
    void IncreaseSuctionConeHeight();

    ///カメラの場所を更新 ///
    void UpdateCameraPos() const;

    ///高さと半径のLerp処理 ///
    void UpdateHeightAndRadiusLerp();
    void UpdateCameraPos();

    float m_coneRadius;
    float m_coneDegree;
    float m_allExp;
    float m_exp;
    bool m_SuctionActive;
    bool m_zoomUp;
    float m_coneTopPos;

    // Lerp用の変数
    LerpValue m_heightLerp;
    LerpValue m_radiusLerp;
    
};


