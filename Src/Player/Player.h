#pragma once
#include "../Common/Object3D.h"
#include "PlayerLevel.h"

// プレイヤーのUFOを制御するクラス //
class CPlayer : public Object3D
{
public:
    // 原点から移動できる距離
    // 例えば-20~20なら20と入力 //
    /// CPlayer を初期化する
    /// @param moveRange moveRange に渡す値
    CPlayer(float moveRange);
    /// CPlayer の終了処理を行う
    ~CPlayer();

    /// With Suction Cone を判定する
    /// @param targetPos 座標
    /// @return 成功または条件を満たす場合 true
    bool IsWithSuctionCone(const VECTOR3& targetPos) const;

    /// 経過時間と対象位置をもとに、吸い込みによって移動する量を計算する
    /// @param moveTimeSecond 経過時間[秒]
    /// @param animalPos 対象オブジェクトの座標
    /// @return 吸い込み移動量
    VECTOR3 CalcSuctionDisplacement(float moveTimeSecond, const VECTOR3& animalPos) const;

    /// Inside Suction Circle を判定する
    /// @param targetPos 座標
    /// @return 成功または条件を満たす場合 true
    bool IsInsideSuctionCircle(const VECTOR3& targetPos) const;

    /// Pos を取得する
    /// @return 3次元ベクトル
    const VECTOR3& GetPos() const { return transform.position; }
    /// Is Suck Up を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetIsSuckUp() const { return m_SuctionActive; }

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

    /// Movement Input を処理する
    void HandleMovementInput();
    /// Camera Pos を毎フレームの状態を更新する
    void UpdateCameraPos();


    bool m_SuctionActive;
    bool m_prevSuctionActive;
    bool m_zoomUp;
    bool m_draw;
    float m_moveRange;

    CPlayerLevel* m_pLevel;
};
