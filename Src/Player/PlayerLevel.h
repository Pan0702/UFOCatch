#pragma once
#include "../Common/Object3D.h"
#include "../Utils/LerpValue.h"

/// <summary>プレイヤーで使う Player Level の情報と処理をまとめる型</summary>
class CPlayerLevel : public Object3D
{
public:
    /// CPlayerLevel を初期化する
    /// @param initialConeTopPos 座標
    /// @param coneDegree coneDegree に渡す値
    CPlayerLevel(float initialConeTopPos, float coneDegree);

    /// CPlayerLevel を初期化する
    CPlayerLevel()
    {
    }

    /// CPlayerLevel の終了処理を行う
    ~CPlayerLevel() = default;

    /// Exp を追加する
    /// @param exp exp に渡す値
    void AddExp(float exp) { m_exp += exp; }
    /// Exp を取得する
    /// @return 計算結果の値
    float GetExp() const { return m_exp; }
    /// All Exp を取得する
    /// @return 計算結果の値
    float GetAllExp() const { return m_allExp; }
    /// Lv を取得する
    /// @return 処理結果の数値
    int GetLv() const { return m_lv; }
    /// Radius を取得する
    /// @return 計算結果の値
    float GetRadius() const { return m_coneRadius; }
    /// Cone Top Pos を取得する
    /// @return 計算結果の値
    float GetConeTopPos() const { return m_coneTopPos; }
    /// Cone Degree を取得する
    /// @return 計算結果の値
    float GetConeDegree() const { return m_coneDegree; }
    /// Raito を取得する
    /// @return 計算結果の値
    float GetRaito() const { return m_raito; }

private:
    /// 毎フレームの状態を更新する
    void Update() override;

    /// 描画する
    void Draw() override
    {
    }

    /// Level を確認する
    void CheckLevel();
    /// Increase Suction Cone Height の処理を行う
    void IncreaseSuctionConeHeight();
    /// Height And Radius Lerp を毎フレームの状態を更新する
    void UpdateHeightAndRadiusLerp();

    float m_exp;
    float m_allExp;
    int m_lv;
    float m_raito;

    float m_coneTopPos;
    float m_coneRadius;
    float m_coneDegree;

    LerpValue m_heightLerp;
    LerpValue m_radiusLerp;
};
