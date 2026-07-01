#pragma once
#include "../Common/Object3D.h"
#include <vector>

/// <summary>汎用ユーティリティで使う Capsule Collider の情報と処理をまとめる型</summary>
class CapsuleCollider
{
public:
    /// <summary>汎用ユーティリティで使う Coll Info の情報と処理をまとめる型</summary>
    struct CollInfo
    {
        VECTOR3 hitPosition; // 衝突した位置
        VECTOR3 normal; // 法線
        VECTOR3 triangle[3]; // 衝突したポリゴンの頂点座標
    };

    /// CapsuleCollider を初期化する
    CapsuleCollider();

    /// Capsule を設定する
    /// @param pos1 座標
    /// @param pos2 座標
    /// @param rad rad に渡す値
    void SetCapsule(VECTOR3 pos1, VECTOR3 pos2, float rad);

    /// Collision Line を確認する
    /// @param trans trans に渡す値
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @param hitOut 衝突情報の出力先
    /// @return 成功または条件を満たす場合 true
    bool CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, CollInfo* hitOut = nullptr);
    /// Collision Triangle を確認する
    /// @param trans trans に渡す値
    /// @param positions 座標
    /// @param hitOut 衝突情報の出力先
    /// @return 成功または条件を満たす場合 true
    bool CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* positions, CollInfo* hitOut = nullptr);

private:
    /// <summary>汎用ユーティリティで使う Capsule Info の情報と処理をまとめる型</summary>
    struct CapsuleInfo
    {
        VECTOR3 position1;
        VECTOR3 position2;
        float radius;
    };
};
