#include "ModelCreator.h"
#include <string>
#include <vector>

#include "Buttom.h"
#include "../Framework/ObjectManager.h"
#include "ConvertFbx/FbxParser.h"
#include "ConvertFbx/MeshWriter.h"

// 謖・ｮ壹ヱ繧ｹ縺ｮ繝｡繝・す繝･繧偵Ο繝ｼ繝峨＠縲√・繧ｿ繝ｳ繝ｪ繧ｹ繝医→繝｢繝・Ν繧ｹ繝医Ξ繝ｼ繧ｸ縺ｫ逋ｻ骭ｲ縺吶ｋ
void ModelCreator::CreateModel(const std::string& path)
{

    const size_t lastSlash = path.find_last_of("\\/");
    const size_t lastDot = path.find_last_of(".");
    
    const size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
    std::string name;
    if (lastDot != std::string::npos && lastDot > start)
    {
        name = path.substr(start, lastDot - start);
    }
    else
    {
        name = path.substr(start);
    }

    for (auto& model : m_models)
    {
        if (model.name == name)
        {
            MessageBox(0, _T("Already Exis"), nullptr, MB_OK);
            return;
        }
    }
    ModelInfo info = {};
    info.name = name;
    info.mesh = new CFbxMesh();
    if (info.mesh->Load(path.c_str()))
    {
        m_models.push_back(info);
        ObjectManager::FindGameObject<Button>()->AddButton(name, info.mesh);
        ResourceManager::LoadFbx(name.c_str(), path.c_str());
    }
}

// FBX 繧・.mesh 縺ｫ螟画鋤縺励※縺九ｉ繝ｭ繝ｼ繝峨☆繧・
void ModelCreator::ConvertAndLoad(const std::string& fbxPath)
{
    // FBX 繧定ｧ｣譫舌＠縺ｦ鬆らせ繝ｻ繧､繝ｳ繝・ャ繧ｯ繧ｹ繧貞叙蠕・
    FbxParser parser;
    if (!parser.Load(fbxPath)) return;

    std::vector<MeshVertex> verts;
    std::vector<uint32_t>   indices;
    if (!parser.ExtractMesh(verts, indices)) return;

    // FBX 縺ｨ蜷後§繝輔か繝ｫ繝繝ｻ蜷悟錐縺ｧ .mesh 縺ｨ縺励※菫晏ｭ・
    std::string meshPath = fbxPath;
    size_t lastDot = meshPath.find_last_of('.');
    if (lastDot != std::string::npos) {
        meshPath.replace(lastDot, meshPath.length() - lastDot, ".mesh");
    } else {
        meshPath += ".mesh";
    }
    std::string texName = parser.GetTextureFileName();
    if (texName.empty())
    {
        MessageBox(0, _T("No Texture"), nullptr, MB_OK);
        return;
    }
    // FbxMesh::Load 縺ｯ "mesh縺ｮ繝・ぅ繝ｬ繧ｯ繝医Μ + 繝・け繧ｹ繝√Ε蜷・ 縺ｧ繝輔Ν繝代せ繧堤ｵ・∩遶九※繧九◆繧√・
    // 繝輔ぃ繧､繝ｫ蜷阪・縺ｿ繧剃ｿ晏ｭ倥☆繧具ｼ育ｵｶ蟇ｾ繝代せ繧・嶌蟇ｾ繝代せ縺ｮ菴呵ｨ医↑諠・ｱ繧帝勁蜴ｻ・・
    {
        size_t pos = texName.find_last_of("/\\");
        if (pos != std::string::npos)
            texName = texName.substr(pos + 1);
    }
    MeshWriter writer;
    if (!writer.Write(meshPath, texName, verts, indices)) return;

    // 螟画鋤縺励◆ .mesh 繧帝壼ｸｸ繝ｭ繝ｼ繝・
    CreateModel(meshPath);
}

