#pragma once
#include <string>
#include <list>

#include "../../Common/Object3D.h"

class CFbxMesh;

// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧ｿ繧､繝・
enum AnimationType
{
    A_IDEL = 0,
    A_WALK,
    A_RUN,
    A_SEACH
};

// 繝｢繝・Ν逋ｻ骭ｲ繝ｻ蜿門ｾ励け繝ｩ繧ｹat
class CModelRegistry : public Object3D
{
public:
    CModelRegistry();
    ~CModelRegistry();

    // 繝｡繝・す繝･繧貞錐蜑阪〒蜿門ｾ・
    CFbxMesh* GetMesh(const std::string& name) const;

    // 繝｡繝・す繝･繧定ｿｽ蜉
    void Register(const std::string& name, CFbxMesh* mesh);

private:
    struct ModelEntry
    {
        std::string name;
        CFbxMesh* mesh;
    };

    std::list<ModelEntry> m_models;

    // 繝・ヵ繧ｩ繝ｫ繝医・繝｡繝・す繝･繧偵Ο繝ｼ繝・
    void LoadDefaultModels();
};

