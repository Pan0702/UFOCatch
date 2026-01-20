#pragma once
#include "../Common/Object3D.h"
#include "../Utils/LerpValue.h"
#include "../Utils/ParticleSystem.h"

// プレイヤーのUFOを制御するクラス //
class CPlayer:public Object3D
{
public:
    // 原点から移動できる距離
    // 例えば-20~20なら20と入力//
    CPlayer(float moveRange);
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
    
    void AddExp(float exp){ m_exp += exp; }
    float GetExp()const {return m_exp;}
    float GetAllExp()const {return m_allExp;}
    int GetLv()const {return m_lv;}
    bool GetIsSuckUp() const { return m_SuctionActive; }

private:

    void Update() override;
    void Draw() override;

    //判定円の描画
    ///Debug///
    void DrawCircle(const VECTOR3& center, float radius, DWORD color);

    //吸い込み円の描画（地面投影）
    void DrawSuctionCircle();
    
    void HandleMovementInput();
    
    ///一定数吸い込んだかチェック ///
    void CheckLevel();
    ///プレイヤーの高さを増加 ///
    void IncreaseSuctionConeHeight();

    ///高さと半径のLerp処理 ///
    void UpdateHeightAndRadiusLerp();
    void UpdateCameraPos();

    float m_coneRadius;
    float m_coneDegree;
    float m_allExp;
    float m_exp;
    int m_lv;
    //吸い込み状態
    bool m_SuctionActive;
    bool m_zoomUp;
    float m_coneTopPos;
    bool m_draw;
    float m_moveRange;

    // Lerp用の変数
    LerpValue m_heightLerp;
    LerpValue m_radiusLerp;

    // 吸い込み円描画用
    CSpriteImage* m_pCircleImage;
    
    CSpriteImage* m_pParticleImage;
    CParticleEmitter* m_pEmitter;

};


