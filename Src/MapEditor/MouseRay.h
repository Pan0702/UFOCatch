#pragma once
#include "../Common/Object3D.h"

/// <summary>
/// ワールド空間上のレイ（半直線）
/// </summary>
struct Ray
{
    VECTOR3 origin; // 始点（カメラ位置）
    VECTOR3 direction; // 正規化済み方向ベクトル
};

/// <summary>
/// マウスカーソルからレイを生成し、オブジェクトとの当たり判定を行うユーティリティクラス
///  Object3Dを継承しないクラスとして定義
/// </summary>
class MouseRay
{
public:
    /// 作成する
    /// @return 処理結果
    static Ray Create();

    /// Hit Test を返す
    /// @param ray 判定に使用するレイ
    /// @param obj 対象オブジェクト
    /// @param collOut 衝突情報の出力先
    /// @param rayLength 判定に使用するレイ
    /// @return 成功または条件を満たす場合 true
    static bool HitTest(const Ray& ray, Object3D* obj,
                        MeshCollider::CollInfo* collOut = nullptr,
                        float rayLength = 1000.0f);

    /// Hit Test Sphere を返す
    /// @param ray 判定に使用するレイ
    /// @param sphere 判定対象の球コライダー
    /// @return 成功または条件を満たす場合 true
    static bool HitTestSphere(const Ray& ray, const SphereCollider& sphere);
};
