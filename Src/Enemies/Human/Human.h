#pragma once

#include "FunShape.h"
#include "../../Common/Object3D.h"
#include "../Base/EnemyBase.h"

/// <summary>敵AIで使う Vision System の情報と処理をまとめる型</summary>
class CVisionSystem;
/// <summary>プレイヤーUFOの移動、吸引、HP、レベル処理をまとめるクラス</summary>
class CPlayer;
/// <summary>敵AIで使う Player HP の情報と処理をまとめる型</summary>
class CPlayerHP;

/// <summary>敵AIで使う Human Base の情報と処理をまとめる型</summary>
class CHumanBase;

/// <summary>敵AIで使う Human の情報と処理をまとめる型</summary>
class CHuman : public CEnemyBase
{
public:
    /// CHuman を初期化する
    /// @param pos 座標
    /// @param areaSize サイズ
    CHuman(const VECTOR3& pos, const VECTOR2& areaSize = VECTOR2(10, 10));
    /// CHuman の終了処理を行う
    ~CHuman();

    /// Pos を追加する
    /// @param pos 座標
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    /// Angle を設定する
    /// @param a a に渡す値
    void SetAngle(float a) { angle = a; }
    /// In Sight を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetInSight() const { return m_inSight; }

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;
    /// Direction Line を描画処理を行う
    // void DrawDirectionLine();
    /// Atk Area の処理を行う
    void AtkArea() const;
    /// States を初期化する
    void InitStates();
    ///
private:
    CFunShape* m_pFunShape;
    DWORD m_dwColor;
    VECTOR2 m_AreaSize;
    float angle;
    bool m_inSight;
};
