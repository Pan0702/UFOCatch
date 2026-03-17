#pragma once
#include <string>
#include "../Utils/FbxMesh.h"

namespace ResourceManager {
	void Init();
	void Reset();
	CFbxMesh* LoadFbx(const std::string& filename);
};