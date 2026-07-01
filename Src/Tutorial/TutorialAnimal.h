#pragma once
#include "../Common/Object3D.h"

/// <summary>チュートリアルで使う Enemy Manager の情報と処理をまとめる型</summary>
class CEnemyManager;

// チュートリアル用の動物クラス //
class CTutorialAnimal : public Object3D
{
public:
    // 指定位置にチュートリアル用動物を生成する
    // @param pos 生成位置 //
    /// CTutorialAnimal を初期化する
    /// @param pos 座標
    CTutorialAnimal(const VECTOR3& pos);
    /// CTutorialAnimal の終了処理を行う
    ~CTutorialAnimal();

    // Scoreを加算してオブジェクトをDestroy //
    /// 破棄する
    void Destroy();

private:
    /// 毎フレームの状態を更新する
    void Update() override;

    // 重力を適用する //
    /// Apply Gravity の処理を行う
    void ApplyGravity();
    /// Bounds2 D を取得する
    /// @param outPos 座標
    /// @param outSize サイズ
    /// @return 成功または条件を満たす場合 true
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    float m_velocityY = 0.0f; // Y軸方向の速度 //
    CEnemyManager* m_pEnemyManager = nullptr;
};
