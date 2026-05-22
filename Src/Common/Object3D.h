#pragma once
#include "../Framework/AudioManager.h"
#include "../Framework/GameObject.h"
#include "../Utils/FbxMesh.h"
#include "../Utils/MeshCollider.h"
#include "../Utils/Animator.h"
#include "Constants.h"

/// <summary>共通オブジェクトで使う Transform の情報と処理をまとめる型</summary>
class Transform
{
public:
    VECTOR3 position;
    VECTOR3 rotation;
    VECTOR3 scale;
    /// Transform を初期化する
    Transform()
    {
        position = VECTOR3(0, 0, 0);
        rotation = VECTOR3(0, 0, 0);
        scale = VECTOR3(1, 1, 1);
    }

    /// matrix を返す
    /// @return 行列
    const MATRIX4X4 matrix() const
    {
        MATRIX4X4 scaleM = XMMatrixScaling(
            scale.x, scale.y, scale.z);
        MATRIX4X4 rotX = XMMatrixRotationX(
            rotation.x);
        MATRIX4X4 rotY = XMMatrixRotationY(
            rotation.y);
        MATRIX4X4 rotZ = XMMatrixRotationZ(
            rotation.z);
        MATRIX4X4 trans = XMMatrixTranslation(
            position.x, position.y, position.z);
        return scaleM * rotZ * rotX * rotY * trans;
    }
};

/// <summary>共通オブジェクトで使う Sphere Collider の情報と処理をまとめる型</summary>
class SphereCollider
{
public:
    VECTOR3 center;
    float radius;
    /// SphereCollider を初期化する
    SphereCollider()
    {
        center = VECTOR3(0, 0, 0);
        radius = 0.0f;
    }
};

/// <summary>Transform、メッシュ、アニメーション、コライダーを持つ3Dオブジェクト基底クラス</summary>
class Object3D : public GameObject
{
public:
    /// Object3D を初期化する
    Object3D();
    /// Object3D の終了処理を行う
    virtual ~Object3D();
    /// 毎フレームの状態を更新する
    virtual void Update() override;
    /// 描画する
    virtual void Draw() override;

    /// Transform を取得する
    /// @return 処理結果
    const Transform GetTransform()
    {
        return transform;
    }

    /// Position を返す
    /// @return 3次元ベクトル
    const VECTOR3 Position()
    {
        return transform.position;
    };
    /// Rotation を返す
    /// @return 3次元ベクトル
    const VECTOR3 Rotation()
    {
        return transform.rotation;
    };
    /// 拡大縮小する
    /// @return 3次元ベクトル
    const VECTOR3 Scale()
    {
        return transform.scale;
    };
    /// Matrix を返す
    /// @return 行列
    const MATRIX4X4 Matrix() { return transform.matrix(); }

    /// Collider を返す
    /// @return 処理結果
    virtual SphereCollider Collider();

    /// Hit Sphere To Mesh Push を返す
    /// @param sphere 判定対象の球コライダー
    /// @param push push に渡す値
    /// @return 成功または条件を満たす場合 true
    virtual bool HitSphereToMeshPush(const SphereCollider& sphere, VECTOR3* push = nullptr);

    /// Hit Sphere To Mesh を返す
    /// @param sphere 判定対象の球コライダー
    /// @param collOut 衝突情報の出力先
    /// @return 成功または条件を満たす場合 true
    virtual bool HitSphereToMesh(const SphereCollider& sphere, MeshCollider::CollInfo* collOut = nullptr);

    /// Hit Line To Mesh を返す
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @param collOut 衝突情報の出力先
    /// @return 成功または条件を満たす場合 true
    virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* collOut = nullptr);

    /// Hit Sphere To Sphere を返す
    /// @param target 対象
    /// @param withY withY に渡す値
    /// @return 計算結果の値
    virtual float HitSphereToSphere(const SphereCollider& target, bool withY = true);

    /// Hit Sphere To Sphere Push を返す
    /// @param target 対象
    /// @param withY withY に渡す値
    /// @param push push に渡す値
    /// @return 成功または条件を満たす場合 true
    virtual bool HitSphereToSpherePush(const SphereCollider& target, bool withY = true, VECTOR3* push = nullptr);

    /// Mesh を取得する
    /// @return 対象のポインタ
    CFbxMesh* GetMesh() { return m_pMesh; }

    /// Mesh Col を返す
    /// @return 対象のポインタ
    MeshCollider* MeshCol() { return m_pMeshCol; }

    /// Animator を取得する
    /// @return 対象のポインタ
    Animator* GetAnimator() { return m_pAnimator.get(); }

    /// Sphere Collider を取得する
    /// @return 処理結果
    SphereCollider GetSphereCollider();

protected:
    CFbxMesh* m_pMesh;
    std::unique_ptr<Animator> m_pAnimator;
    MeshCollider* m_pMeshCol;
    Transform transform;
};
