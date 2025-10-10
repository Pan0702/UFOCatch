#pragma once
#include "Object3D.h"
#include "FbxMesh.h"

class SphereCollider
{
public:
	SphereCollider();
	SphereCollider(Object3D* object);
	void MakeFromMesh(CFbxMesh* mesh);
	void MakeFromFile(std::string fileName);
	VECTOR3 Center() { return center; }
	float Radius() { return radius; }
private:
	VECTOR3 center;
	float radius;
};