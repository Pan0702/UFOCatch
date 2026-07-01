#include "ModelCreator.h"
#include <string>
#include <vector>

#include "Buttom.h"
#include "../Framework/ObjectManager.h"
#include "ConvertFbx/FbxParser.h"
#include "ConvertFbx/MeshWriter.h"

// 指定パスのメッシュをロードし、ボタンリストとモデルストレージに登録する
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

// FBX を .mesh に変換してからロードする
void ModelCreator::ConvertAndLoad(const std::string& fbxPath)
{
    FbxParser parser;
    if (!parser.Load(fbxPath)) return;

    std::vector<MeshVertex> verts;
    std::vector<uint32_t> indices;
    if (!parser.ExtractMesh(verts, indices)) return;

    std::string meshPath = fbxPath;
    size_t lastDot = meshPath.find_last_of('.');
    if (lastDot != std::string::npos)
    {
        meshPath.replace(lastDot, meshPath.length() - lastDot, ".mesh");
    }
    else
    {
        meshPath += ".mesh";
    }
    std::string texName = parser.GetTextureFileName();
    if (texName.empty())
    {
        MessageBox(0, _T("No Texture"), nullptr, MB_OK);
        return;
    }
    // FbxMesh::Load は "meshのディレクトリ + テクスチャ名" でフルパスを組み立てるため、
    // ファイル名のみを保存する（絶対パスや相対パスの余計な情報を除去）
    {
        size_t pos = texName.find_last_of("/\\");
        if (pos != std::string::npos)
            texName = texName.substr(pos + 1);
    }
    MeshWriter writer;
    if (!writer.Write(meshPath, texName, verts, indices)) return;

    // 変換した .mesh を通常ロード
    CreateModel(meshPath);
}
