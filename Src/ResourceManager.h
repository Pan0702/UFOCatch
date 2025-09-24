#pragma once
#include <string>
#include "FbxMesh.h"

namespace ResourceManager {
	void Init();
	void Reset();
	CFbxMesh* LoadFbx(std::string filename);
};