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
	if (push != nullptr) { // 座標が必要なので
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // 最終的に押し戻すベクトル
		for (const MeshCollider::CollInfo& m : meshes) { // 接触しているすべてのポリゴン
			VECTOR3 move = sphere.center - m.hitPosition;
			float len = move.Length(); // 接触した点から中心への距離
			move = move * ((sphere.radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // 押し戻すためのベクトル
			// これをpushVecと合成する
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // 押し戻し済みベクトルの向き
			float dot = Dot(push, pushVecNorm);	// その成分の長さ
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // その成分を差し引いて足す
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
	if (pushVec.LengthSquare() < rsum * rsum) {	  // 球の当たり判定
		// 接触してるとき
		if (push != nullptr) {
			// 押し出し方向pushVec  押し出し距離pushLen
			// 押し出しベクトルpushを求める
			float pushLen = rsum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVecの長さを１にする
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
}
