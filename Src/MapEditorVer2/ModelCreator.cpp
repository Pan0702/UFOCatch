#include "ModelCreator.h"
#include <string>
#include <vector>

#include "Buttom.h"
#include "../ModelStorage.h"
#include "../ObjectManager.h"
#include "ConvertFbx/FbxParser.h"
#include "ConvertFbx/MeshWriter.h"

// 指定パスのメッシュをロードし、ボタンリストとモデルストレージに登録する
void ModelCreator::CreateModel(const std::string& path)
{

    size_t last_slash = path.find_last_of("\\/");
    size_t last_dot = path.find_last_of(".");
    
    size_t start = (last_slash == std::string::npos) ? 0 : last_slash + 1;
    std::string name;
    if (last_dot != std::string::npos && last_dot > start)
    {
        name = path.substr(start, last_dot - start);
    }
    else
    {
        name = path.substr(start);
    }

    for (auto& model : models_)
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
        models_.push_back(info);
        ObjectManager::FindGameObject<Button>()->AddButton(name, info.mesh);
        ObjectManager::FindGameObject<CModelStorage>()->AddModel(name.c_str(), path.c_str());
    }
}

// FBX を .mesh に変換してからロードする
void ModelCreator::ConvertAndLoad(const std::string& fbx_path)
{
    // FBX を解析して頂点・インデックスを取得
    FbxParser parser;
    if (!parser.Load(fbx_path)) return;

    std::vector<MeshVertex> verts;
    std::vector<uint32_t>   indices;
    if (!parser.ExtractMesh(verts, indices)) return;

    // FBX と同じフォルダ・同名で .mesh として保存
    std::string mesh_path = fbx_path;
    size_t last_dot = mesh_path.find_last_of('.');
    if (last_dot != std::string::npos) {
        mesh_path.replace(last_dot, mesh_path.length() - last_dot, ".mesh");
    } else {
        mesh_path += ".mesh";
    }
    std::string tex_name = parser.GetTextureFileName();
    if (tex_name.empty())
    {
        MessageBox(0, _T("No Texture"), nullptr, MB_OK);
        return;
    }
    // FbxMesh::Load は "meshのディレクトリ + テクスチャ名" でフルパスを組み立てるため、
    // ファイル名のみを保存する（絶対パスや相対パスの余計な情報を除去）
    {
        size_t pos = tex_name.find_last_of("/\\");
        if (pos != std::string::npos)
            tex_name = tex_name.substr(pos + 1);
    }
    MeshWriter writer;
    if (!writer.Write(mesh_path, tex_name, verts, indices)) return;

    // 変換した .mesh を通常ロード
    CreateModel(mesh_path);
}
