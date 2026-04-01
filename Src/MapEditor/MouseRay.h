#pragma once
#include "../Common/Object3D.h"

/// <summary>
/// ワールド空間上のレイ（半直線）
/// </summary>
struct Ray
{
    VECTOR3 origin;    // 始点（カメラ位置）
    VECTOR3 direction; // 正規化済み方向ベクトル
};

/// <summary>
/// マウスカーソルからレイを生成し、オブジェクトとの当たり判定を行うユーティリティクラス
///  Object3Dを継承しないクラスとして定義
/// </summary>
class MouseRay
{
public:
    /// <summary>
    /// 現在のマウスカーソル位置からワールド空間のレイを生成する
    /// </summary>
    /// <returns>生成されたRay</returns>
    static Ray Create();

    /// <summary>
    /// レイとObject3Dのメッシュの当たり判定
    /// </summary>
    /// <param name="ray">判定するレイ</param>
    /// <param name="obj">判定対象のObject3D</param>
    /// <param name="collOut">衝突情報の出力先（不要な場合はnullptr）</param>
    /// <param name="rayLength">レイの長さ（デフォルト1000）</param>
    /// <returns>当たっていればtrue</returns>
    static bool HitTest(const Ray& ray, Object3D* obj,
                        MeshCollider::CollInfo* collOut = nullptr,
                        float rayLength = 1000.0f);

    /// <summary>
    /// レイと球の当たり判定
    /// </summary>
    /// <param name="ray">判定するレイ</param>
    /// <param name="sphere">判定対象の球コライダ</param>
    /// <returns>当たっていればtrue</returns>
    static bool HitTestSphere(const Ray& ray, const SphereCollider& sphere);
};