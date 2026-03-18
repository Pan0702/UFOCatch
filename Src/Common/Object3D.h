#pragma once
#include "../Framework/AudioManager.h"
#include "../Framework/GameObject.h"
#include "../Utils/FbxMesh.h"
#include "../Utils/MeshCollider.h"
#include "../Utils/Animator.h"

class Transform {
public:
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	Transform() {
		position = VECTOR3(0, 0, 0);
		rotation = VECTOR3(0, 0, 0);   // ・ｽ・ｽ・ｽW・ｽA・ｽ・ｽ・ｽp
		scale = VECTOR3(1, 1, 1);
	}
	const MATRIX4X4 matrix() const {
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
	/// ・ｽ・ｽ・ｽﾆ・ｿｽ・ｽb・ｽV・ｽ・ｽ・ｽﾌ難ｿｽ・ｽ・ｽ・ｽ阡ｻ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	/// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽﾉのみ、push・ｽﾉ会ｿｽ・ｽ・ｽ・ｽﾔゑｿｽ・ｽ齒奇ｿｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <param name="sphere">・ｽ・ｽ・ｽ・ｽ</param>
	/// <param name="push">・ｽ・ｽ・ｽ・ｽ・ｽﾔゑｿｽ・ｽ・ｽ・ｽW・ｽ・ｽ・ｽi・ｽ[・ｽ・ｽ・ｽ・ｽ齒・/param>
	/// <returns>・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽ・ｽtrue</returns>
	virtual bool HitSphereToMeshPush(const SphereCollider& sphere, VECTOR3* push = nullptr);

	/// <summary>
	/// ・ｽ・ｽ・ｽﾆ・ｿｽ・ｽb・ｽV・ｽ・ｽ・ｽﾌ難ｿｽ・ｽ・ｽ・ｽ阡ｻ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	/// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽﾍ、・ｽﾕ突擾ｿｽ・ｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <param name="sphere">・ｽ・ｽ・ｽ・ｽ</param>
	/// <param name="collOut">・ｽﾕ突擾ｿｽ・ｽ・ｽ・ｽi・ｽ[・ｽ・ｽ・ｽ・ｽ齒・/param>
	/// <returns>・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽ・ｽtrue</returns>
	virtual bool HitSphereToMesh(const SphereCollider& sphere, MeshCollider::CollInfo* collOut = nullptr);

	/// <summary>
	/// ・ｽ・ｽ・ｽ・ｽ・ｽﾆ・ｿｽ・ｽb・ｽV・ｽ・ｽ・ｽﾌ難ｿｽ・ｽ・ｽ・ｽ阡ｻ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	/// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽﾍ、・ｽﾕ突擾ｿｽ・ｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <param name="from">・ｽ・ｽ・ｽ・ｽ・ｽﾌ始・ｽ_</param>
	/// <param name="to">・ｽ・ｽ・ｽ・ｽ・ｽﾌ終・ｽ_</param>
	/// <param name="collOut">・ｽﾕ突擾ｿｽ・ｽ・ｽ・ｽi・ｽ[・ｽ・ｽ・ｽ・ｽ齒・/param>
	/// <returns>・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽ・ｽtrue</returns>
	virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* collOut = nullptr);

	/// <summary>
	/// ・ｽ・ｽ・ｽﾆ具ｿｽ・ｽﾌ難ｿｽ・ｽ・ｽ・ｽ阡ｻ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	/// ・ｽ・ｽ・ｽ・ｽ・ｽﾌ具ｿｽ・ｽﾍ、Collider()・ｽﾅ取得・ｽ・ｽ・ｽ・ｽ
	/// </summary>
	/// <param name="target">・ｽ・ｽ・ｽ・ｽﾌ具ｿｽ</param>
	/// <param name="withY">false・ｽﾉゑｿｽ・ｽ・ｽ・ｽY・ｽﾌ搾ｿｽ・ｽW・ｽ・ｽ・ｽｳ趣ｿｽ・ｽ・ｽ・ｽ・ｽ</param>
	/// <returns>・ｽd・ｽﾈゑｿｽ・ｽ</returns>
	virtual float HitSphereToSphere(const SphereCollider& target, bool withY=true);

	/// <summary>
	/// ・ｽ・ｽ・ｽﾆ具ｿｽ・ｽﾌ難ｿｽ・ｽ・ｽ・ｽ阡ｻ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	/// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽﾌ、push・ｽﾉ会ｿｽ・ｽ・ｽ・ｽﾔゑｿｽ・ｽ齒奇ｿｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <param name="target">・ｽ・ｽ・ｽ・ｽﾌ具ｿｽ</param>
	/// <param name="withY">false・ｽﾉゑｿｽ・ｽ・ｽ・ｽY・ｽﾌ搾ｿｽ・ｽW・ｽ・ｽ・ｽｳ趣ｿｽ・ｽ・ｽ・ｽ・ｽ</param>
	/// <returns>・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ鼾・ｿｽ・ｽtrue</returns>
	virtual bool HitSphereToSpherePush(const SphereCollider& target, bool withY=true, VECTOR3* push = nullptr);

	/// <summary>
	/// ・ｽ・ｽ・ｽb・ｽV・ｽ・ｽ・ｽﾌア・ｽh・ｽ・ｽ・ｽX・ｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <returns>・ｽ・ｽ・ｽb・ｽV・ｽ・ｽ・ｽﾌア・ｽh・ｽ・ｽ・ｽX</returns>
	CFbxMesh* GetMesh() { return m_pMesh; }

	/// <summary>
	/// ・ｽR・ｽ・ｽ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽb・ｽV・ｽ・ｽ・ｽﾌア・ｽh・ｽ・ｽ・ｽX・ｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <returns>・ｽR・ｽ・ｽ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽb・ｽV・ｽ・ｽ・ｽﾌア・ｽh・ｽ・ｽ・ｽX</returns>
	MeshCollider* MeshCol() { return m_pMeshCol; }

	/// <summary>
	/// ・ｽA・ｽj・ｽ・ｽ・ｽ[・ｽ^・ｽ[・ｽﾌア・ｽh・ｽ・ｽ・ｽX・ｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <returns>・ｽA・ｽj・ｽ・ｽ・ｽ[・ｽ^・ｽ[・ｽﾌア・ｽh・ｽ・ｽ・ｽX</returns>
	Animator* GetAnimator() { return m_pAnimator.get(); }

	/// <summary>
	/// ・ｽR・ｽ・ｽ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽb・ｽV・ｽ・ｽ・ｽ・ｽBall・ｽ・ｽ・ｽ・ｽﾔゑｿｽ
	/// </summary>
	/// <returns>SphereCollider</returns>
	SphereCollider  GetSphereCollider();

protected:
	CFbxMesh* m_pMesh;
	std::unique_ptr<Animator> m_pAnimator;
	MeshCollider* m_pMeshCol;
	Transform transform;
};
