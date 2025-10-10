#pragma once
#include <string>
#include "../GameLib/FbxMesh.h"

namespace ResourceManager {
	void Init();
	void Reset();
	CFbxMesh* LoadFbx(std::string filename);
};