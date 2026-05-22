#pragma once

#include "../../Common/Object3D.h"
#include "../Base/EnemyBase.h"
#include "../../Player/Player.h"


/// <summary>敵AIで使う Animal Chicken の情報と処理をまとめる型</summary>
class CAnimalChicken : public CEnemyBase
{
public:
    /// CAnimalChicken を初期化する
    /// @param iniPos 初期座標
    /// @param moveAreaSize サイズ
    CAnimalChicken(const VECTOR3& iniPos = VECTOR3(0, 0, 0), const VECTOR2& moveAreaSize = VECTOR2(10, 10));
    /// CAnimalChicken の終了処理を行う
    ~CAnimalChicken();

    /// Suction Check を判定する
    void IsSuctionCheck();

    /// Suction Speed を返す
    /// @return 3次元ベクトル
    VECTOR3 SuctionSpeed() const override;

    /// Pos を追加する
    /// @param pos 座標
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    /// Audio を取得する
    /// @return 対象のポインタ
    CXAudioSource* GetAudio() const { return m_pCry; }
    /// State を切り替える
    /// @param type type に渡す値
    void ChangeState(CBaseState::State type) override;
    /// Cry の処理を行う
    void Cry() const;

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
    const VECTOR2 m_areaSize;
    CPlayer* m_pPlayer;
    CXAudioSource* m_pCry;
};
