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
		rotation = VECTOR3(0, 0, 0);   // ���W�A���p
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
	/// ���ƃ��b�V���̓����蔻�������
	/// ���������ꍇ�ɂ̂݁Apush�ɉ����Ԃ��ꏊ��Ԃ�
	/// </summary>
	/// <param name="sphere">����</param>
	/// <param name="push">�����Ԃ����W���i�[����ꏊ</param>
	/// <returns>���������ꍇ��true</returns>
	virtual bool HitSphereToMeshPush(const SphereCollider& sphere, VECTOR3* push = nullptr);

	/// <summary>
	/// ���ƃ��b�V���̓����蔻�������
	/// ���������ꍇ�́A�Փˏ���Ԃ�
	/// </summary>
	/// <param name="sphere">����</param>
	/// <param name="collOut">�Փˏ����i�[����ꏊ</param>
	/// <returns>���������ꍇ��true</returns>
	virtual bool HitSphereToMesh(const SphereCollider& sphere, MeshCollider::CollInfo* collOut = nullptr);

	/// <summary>
	/// �����ƃ��b�V���̓����蔻�������
	/// ���������ꍇ�́A�Փˏ���Ԃ�
	/// </summary>
	/// <param name="from">�����̎n�_</param>
	/// <param name="to">�����̏I�_</param>
	/// <param name="collOut">�Փˏ����i�[����ꏊ</param>
	/// <returns>���������ꍇ��true</returns>
	virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* collOut = nullptr);

	/// <summary>
	/// ���Ƌ��̓����蔻�������
	/// �����̋��́ACollider()�Ŏ擾����
	/// </summary>
	/// <param name="target">����̋�</param>
	/// <param name="withY">false�ɂ����Y�̍��W���𖳎�����</param>
	/// <returns>�d�Ȃ��</returns>
	virtual float HitSphereToSphere(const SphereCollider& target, bool withY=true);

	/// <summary>
	/// ���Ƌ��̓����蔻�������
	/// ���������ꍇ�́Apush�ɉ����Ԃ��ꏊ��Ԃ�
	/// </summary>
	/// <param name="target">����̋�</param>
	/// <param name="withY">false�ɂ����Y�̍��W���𖳎�����</param>
	/// <returns>���������ꍇ��true</returns>
	virtual bool HitSphereToSpherePush(const SphereCollider& target, bool withY=true, VECTOR3* push = nullptr);

	/// <summary>
	/// ���b�V���̃A�h���X��Ԃ�
	/// </summary>
	/// <returns>���b�V���̃A�h���X</returns>
	CFbxMesh* GetMesh() { return m_pMesh; }

	/// <summary>
	/// �R���W�������b�V���̃A�h���X��Ԃ�
	/// </summary>
	/// <returns>�R���W�������b�V���̃A�h���X</returns>
	MeshCollider* MeshCol() { return m_pMeshCol; }

	/// <summary>
	/// �A�j���[�^�[�̃A�h���X��Ԃ�
	/// </summary>
	/// <returns>�A�j���[�^�[�̃A�h���X</returns>
	Animator* GetAnimator() { return m_pAnimator; }

	/// <summary>
	/// �R���W�������b�V����Ball����Ԃ�
	/// </summary>
	/// <returns>SphereCollider</returns>
	SphereCollider  GetSphereCollider();

protected:
	CFbxMesh* m_pMesh;
	Animator* m_pAnimator;
	MeshCollider* m_pMeshCol;
	Transform transform;
};