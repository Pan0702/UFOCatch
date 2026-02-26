#include "Object3D.h"
#include "../Utils/MeshCollider.h"
#include "../Utils/Animator.h"

Object3D::Object3D()
{
	m_pAnimator = nullptr;
	m_pMesh = nullptr;
	m_pMeshCol = nullptr;
}

Object3D::~Object3D()
{
	// mesh と meshCol は各クラスで管理されるため削除しない
}

void Object3D::Update()
{
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->Update();
	}
}

void Object3D::Draw()
{
	if (m_pMesh == nullptr)  return;
	if (m_pAnimator == nullptr)
	{
		m_pMesh->Render(transform.matrix());
	}
	else {
		m_pMesh->Render(m_pAnimator.get(), transform.matrix());
	}

}

SphereCollider Object3D::Collider()
{
	SphereCollider sphere;
	sphere.center = transform.position + VECTOR3(0, 1, 0);
	sphere.radius = 0.5f;
	return sphere;
}

bool Object3D::HitSphereToMeshPush(const SphereCollider& sphere, VECTOR3* push)
{
	if (m_pMeshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	std::list<MeshCollider::CollInfo> meshes = m_pMeshCol->CheckCollisionSphereList(mat, sphere.center, sphere.radius);
	if (meshes.size() == 0)
		return false;
	if (push != nullptr) { // ���W���K�v�Ȃ̂ł����
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // �ŏI�I�ɉ����x�N�g��
		for (const MeshCollider::CollInfo& m : meshes) { // �������Ă��邷�ׂẴ|���S����
			VECTOR3 move = sphere.center - m.hitPosition;
			float len = move.Length(); // ���������_���璆�S�ւ̋���
			move = move * ((sphere.radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // �����Ԃ������x�N�g��
			// ����pushVec�ƍ�������
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // �����ς݃x�N�g���̌���
			float dot = Dot(push, pushVecNorm);	// ���̐����̒���
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // ���̐��������炵�Ă���
			}
			else {
				pushVec = push;
			}
		}
		*push = pushVec;
	}
	return true;
}


bool Object3D::HitSphereToMesh(const SphereCollider& sphere, MeshCollider::CollInfo* collOut)
{
	if (m_pMeshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	MeshCollider::CollInfo coll;
	bool ret = m_pMeshCol->CheckCollisionSphere(mat, sphere.center, sphere.radius, &coll);
	if (ret) {
		if (collOut != nullptr) {
			*collOut = coll;
		}
		return true;
	}
	return false;
}

bool Object3D::HitLineToMesh(const VECTOR3& from, const VECTOR3& to, MeshCollider::CollInfo* collOut)
{
	if (m_pMeshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	MeshCollider::CollInfo coll;
	bool ret = m_pMeshCol->CheckCollisionLine(mat, from, to, &coll);
	if (ret) {
		if (collOut != nullptr) {
			*collOut = coll;
		}
		return true;
	}
	return false;
}

float Object3D::HitSphereToSphere(const SphereCollider& target, bool withY)
{
	SphereCollider my = Collider();
	VECTOR3 diff = target.center - my.center;
	if (withY == false)
		diff.y = 0.0f;
	float rsum = target.radius + my.radius;
	if (diff.LengthSquare() < rsum * rsum) {
		return rsum - diff.Length();
	}
	return 0.0f;
}

bool Object3D::HitSphereToSpherePush(const SphereCollider& target, bool withY, VECTOR3* push)
{
	SphereCollider my = Collider();
	VECTOR3 pushVec = my.center - target.center;
	if (withY == false)
		pushVec.y = 0.0f;
	float rsum = my.radius + target.radius;
	if (pushVec.LengthSquare() < rsum * rsum) {	  // ���̓����蔻��
		// �������Ă�Ƃ�
		if (push != nullptr) {
			// �����o��������pushVec  �����o��������pushLen
			// �����o���x�N�g��push�����߂�
			float pushLen = rsum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			*push = pushVec * pushLen;
		}
		return true;
	}
	return false;
}

SphereCollider  Object3D::GetSphereCollider()
{
	SphereCollider col;
	if (m_pMeshCol != nullptr)
	{
		m_pMeshCol->GetBall(&col.center, &col.radius);
	}
	return col;
}//　