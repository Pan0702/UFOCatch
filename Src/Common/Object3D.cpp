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
	// mesh 縺ｨ meshCol 縺ｯ蜷・け繝ｩ繧ｹ縺ｧ邂｡逅・＆繧後ｋ縺溘ａ蜑企勁縺励↑縺・
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
	if (push != nullptr) { // ・ｽ・ｽ・ｽW・ｽ・ｽ・ｽK・ｽv・ｽﾈのでゑｿｽ・ｽ・ｽ・ｽ
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // ・ｽﾅ終・ｽI・ｽﾉ会ｿｽ・ｽ・ｽ・ｽx・ｽN・ｽg・ｽ・ｽ
		for (const MeshCollider::CollInfo& m : meshes) { // ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾄゑｿｽ・ｽ驍ｷ・ｽﾗてのポ・ｽ・ｽ・ｽS・ｽ・ｽ・ｽ・ｽ
			VECTOR3 move = sphere.center - m.hitPosition;
			float len = move.Length(); // ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ_・ｽ・ｽ・ｽ迺・ｿｽS・ｽﾖの具ｿｽ・ｽ・ｽ
			move = move * ((sphere.radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // ・ｽ・ｽ・ｽ・ｽ・ｽﾔゑｿｽ・ｽ・ｽ・ｽ・ｽ・ｽx・ｽN・ｽg・ｽ・ｽ
			// ・ｽ・ｽ・ｽ・ｽpushVec・ｽﾆ搾ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // ・ｽ・ｽ・ｽ・ｽ・ｽﾏみベ・ｽN・ｽg・ｽ・ｽ・ｽﾌ鯉ｿｽ・ｽ・ｽ
			float dot = Dot(push, pushVecNorm);	// ・ｽ・ｽ・ｽﾌ撰ｿｽ・ｽ・ｽ・ｽﾌ抵ｿｽ・ｽ・ｽ
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // ・ｽ・ｽ・ｽﾌ撰ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ轤ｵ・ｽﾄゑｿｽ・ｽ・ｽ
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
	if (pushVec.LengthSquare() < rsum * rsum) {	  // ・ｽ・ｽ・ｽﾌ難ｿｽ・ｽ・ｽ・ｽ阡ｻ・ｽ・ｽ
		// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾄゑｿｽﾆゑｿｽ
		if (push != nullptr) {
			// ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽpushVec  ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽpushLen
			// ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽ・ｽ・ｽx・ｽN・ｽg・ｽ・ｽpush・ｽ・ｽ・ｽ・ｽ・ｽﾟゑｿｽ
			float pushLen = rsum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVec・ｽﾌ抵ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽP・ｽﾉゑｿｽ・ｽ・ｽ
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
}//縲