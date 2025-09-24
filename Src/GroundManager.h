#pragma once
#include "Object3D.h"
#include "MeshCollider.h"

class Ground : public Object3D
{
public:
	Ground();
	~Ground();
	bool HitCheckSphere(const VECTOR3& center, float radius, VECTOR3* hit = nullptr);
	bool HitCheckLine(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit = nullptr);
private:
	MeshCollider* meshCol;
};