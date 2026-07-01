#pragma once
#include "../Base/EnemyBase.h"
/// <summary>プレイヤーUFOの移動、吸引、HP、レベル処理をまとめるクラス</summary>
class CPlayer;

/// <summary>敵AIで使う ADebug の情報と処理をまとめる型</summary>
class CADebug : public CEnemyBase
{
public:
    /// CADebug を初期化する
    CADebug();
    /// CADebug を初期化する
    /// @param iniPos 初期座標
    /// @param moveAreaSize サイズ
    CADebug(const VECTOR3& iniPos, const VECTOR2& moveAreaSize);
    /// CADebug の終了処理を行う
    ~CADebug();

private:
    /// Suction Check を判定する
    void IsSuctionCheck();

    /// Suction Speed を返す
    /// @return 3次元ベクトル
    VECTOR3 SuctionSpeed() const override;

    /// Pos を追加する
    /// @param pos 座標
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;
    /// Should Apply Gravity を返す
    /// @return 成功または条件を満たす場合 true
    bool ShouldApplyGravity() const override;

private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    CPlayer* m_pPlayer;
};
