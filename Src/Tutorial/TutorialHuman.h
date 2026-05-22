#pragma once
#include "TutorialAnimal.h"

/// <summary>チュートリアルで使う Fun Shape の情報と処理をまとめる型</summary>
class CFunShape;

// チュートリアル用の人間クラス //
class CTutorialHuman : public Object3D
{
public:
    // 指定位置にチュートリアル用人間を生成する
    // @param pos 生成位置 //
    /// CTutorialHuman を初期化する
    /// @param pos 座標
    CTutorialHuman(const VECTOR3& pos);
    /// CTutorialHuman の終了処理を行う
    ~CTutorialHuman();
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Vision Shape を毎フレームの状態を更新する
    void UpdateVisionShape() const;

    bool m_inSight; // プレイヤーが視界内にいるかどうか //
    CFunShape* m_pFunShape = nullptr;
};
