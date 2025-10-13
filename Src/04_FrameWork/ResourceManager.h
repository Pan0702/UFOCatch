#pragma once
#include <string>
#include "../06_GameLib/FbxMesh.h"

namespace ResourceManager {
	void Init();
	void Reset();
	CFbxMesh* LoadFbx(std::string filename);
};