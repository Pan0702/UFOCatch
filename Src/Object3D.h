#pragma once
#include "GameObject.h"
#include "FbxMesh.h"
#include "MeshCollider.h"
#include "Animator.h"

class Transform {
public:
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	Transform() {
		position = VECTOR3(0, 0, 0);
		rotation = VECTOR3(0, 0, 0);   // ラジアン角
		scale = VECTOR3(1, 1, 1);
	}
	const MATRIX4X4 matrix() {
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

class SphereCollider {
public:
	VECTOR3 center;
	float radius;
	SphereCollider() {
		center = VECTOR3(0, 0, 0);
		radius = 0.0f;
	}
};

class Object3D : public GameObject {
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update() override;
	virtual void Draw() override;

	const Transform GetTransform() {
		return transform;
	}
	const VECTOR3 Position() {
		return transform.position;
	};
	const VECTOR3 Rotation() {
		return transform.rotation;
	};
	const VECTOR3 Scale() {
		return transform.scale;
	};
	const MATRIX4X4 Matrix() { return transform.matrix(); }

	virtual SphereCollider Collider();

	/// <summary>
	/// 球とメッシュの当たり判定をする
	/// 当たった場合にのみ、pushに押し返す場所を返す
	/// </summary>
	/// <param name="sphere">球体</param>
	/// <param name="push">押し返す座標を格納する場所</param>
	/// <returns>当たった場合にtrue</returns>
	virtual bool HitSphereToMeshPush(const SphereCollider& sphere, VECTOR3* push = nullptr);

	/// <summary>
	/// 球とメッシュの当たり判定をする
	/// 当たった場合は、衝突情報を返す
	/// </summary>
	/// <param name="sphere">球体</param>
	/// <param name="collOut">衝突情報を格納する場所</param>
	/// <returns>当たった場合にtrue</returns>
	virtual bool HitSphereToMesh(const SphereCollider& sphere, MeshCollider::CollInfo* collOut = nullptr);

	/// <summary>
	/// 直線とメッシュの当たり判定をする
	/// 当たった場合は、衝突情報を返す
	/// </summary>
	/// <param name="from">直線の始点</param>
	/// <param name="to">直線の終点</param>
	/// <param name="collOut">衝突情報を格納する場所</param>
	/// <returns>当たった場合にtrue</returns>
	virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* collOut = nullptr);

	/// <summary>
	/// 球と球の当たり判定をする
	/// 自分の球は、Collider()で取得する
	/// </summary>
	/// <param name="target">相手の球</param>
	/// <param name="withY">falseにするとYの座標差を無視する</param>
	/// <returns>重なり量</returns>
	virtual float HitSphereToSphere(const SphereCollider& target, bool withY=true);

	/// <summary>
	/// 球と球の当たり判定をする
	/// 当たった場合は、pushに押し返す場所を返す
	/// </summary>
	/// <param name="target">相手の球</param>
	/// <param name="withY">falseにするとYの座標差を無視する</param>
	/// <returns>当たった場合にtrue</returns>
	virtual bool HitSphereToSpherePush(const SphereCollider& target, bool withY=true, VECTOR3* push = nullptr);

	/// <summary>
	/// メッシュのアドレスを返す
	/// </summary>
	/// <returns>メッシュのアドレス</returns>
	CFbxMesh* Mesh() { return mesh; }

	/// <summary>
	/// コリジョンメッシュのアドレスを返す
	/// </summary>
	/// <returns>コリジョンメッシュのアドレス</returns>
	MeshCollider* MeshCol() { return meshCol; }

	/// <summary>
	/// アニメーターのアドレスを返す
	/// </summary>
	/// <returns>アニメーターのアドレス</returns>
	Animator* GetAnimator() { return animator; }

	/// <summary>
	/// コリジョンメッシュのBall情報を返す
	/// </summary>
	/// <returns>SphereCollider</returns>
	SphereCollider  GetSphereCollider();

protected:
	CFbxMesh* mesh;
	Animator* animator;
	MeshCollider* meshCol;
	Transform transform;
};