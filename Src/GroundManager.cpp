#include "Ground.h"

Ground::Ground()
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Map/map50Field1.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
}

Ground::~Ground()
{
	SAFE_DELETE(meshCol);
	SAFE_DELETE(mesh);
}

bool Ground::HitCheckSphere(const VECTOR3& center, float radius, VECTOR3* hit)
{
	std::list<MeshCollider::CollInfo> meshes = meshCol->CheckCollisionSphereList(XMMatrixIdentity(), center, radius);
	if (meshes.size() == 0)
		return false;
	if (hit != nullptr) { // 座標が必要なのであれば
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // 最終的に押すベクトル
		for (const MeshCollider::CollInfo& m : meshes) { // 当たっているすべてのポリゴンで
			VECTOR3 move = center - m.hitPosition;
			float len = move.Length(); // 当たった点から中心への距離
			move = move * ((radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // 押し返したいベクトル
			// 今のpushVecと合成する
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // 合成済みベクトルの向き
			float dot = Dot(push, pushVecNorm);	// その成分の長さ
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // その成分を減らしていい
			}
			else {
				pushVec = push;
			}
		}
		float f = pushVec.Length();
		*hit = center + pushVec;
	}
	return true;
}

bool Ground::HitCheckLine(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit)
{
	MeshCollider::CollInfo coll;
	bool ret = meshCol->CheckCollisionLine(XMMatrixIdentity(), from, to, &coll);
	if (ret) {
		if (hit != nullptr) {
			*hit = coll.hitPosition;
		}
		return true;
	}
	return false;
}
