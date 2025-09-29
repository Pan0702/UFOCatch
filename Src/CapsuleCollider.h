#pragma once
#include "Object3D.h"
#include <vector>

class CapsuleCollider
{
public:
	struct CollInfo {
		VECTOR3 hitPosition; // ���������ʒu
		VECTOR3 normal; // �@��
		VECTOR3 triangle[3]; // ���_
	};

	CapsuleCollider();

	void SetCapsule(VECTOR3 pos1, VECTOR3 pos2, float rad);

	bool CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, CollInfo* hitOut = nullptr);
	bool CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* positions, CollInfo* hitOut = nullptr);

private:
	struct CapsuleInfo {
		VECTOR3 position1;
		VECTOR3 position2;
		float radius;
	};
};