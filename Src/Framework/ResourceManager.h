#pragma once
#include <string>
#include "../Utils/FbxMesh.h"
#include "../Utils/MeshCollider.h"

namespace ResourceManager
{
    /// 初期化する
    void Init();
    /// 初期状態に戻す
    void Reset();
    /// Fbx を読み込む
    /// @param filename ファイル名
    /// @return 対象のポインタ
    CFbxMesh* LoadFbx(const std::string& filename);
    /// Fbx を読み込む
    /// @param name 名前
    /// @param path パス
    void LoadFbx(const char* name, const char* path);
    /// Fbx を読み込む
    /// @param paths パス
    void LoadFbx(const std::vector<std::string>& paths);
    /// Fbx を読み込む
    /// @param path パス
    /// @return 対象のポインタ
    CFbxMesh* LoadFbx(const char* path);
    /// Model を取得する
    /// @param name 名前
    /// @return 対象のポインタ
    CFbxMesh* GetModel(const char* name);
    /// Model Names を取得する
    /// @return 取得した要素一覧
    std::vector<std::string> GetModelNames();
    /// Coll を取得する
    /// @param name 名前
    /// @return 対象のポインタ
    MeshCollider* GetColl(const char* name);
    /// Coll を取得する
    /// @param mesh mesh に渡す値
    /// @return 対象のポインタ
    MeshCollider* GetColl(const CFbxMesh* mesh);
    /// Path を取得する
    /// @param name 名前
    /// @return 対象のポインタ
    const char* GetPath(const char* name);
    /// All Model Path を取得する
    /// @return 取得した要素一覧
    std::vector<std::string> GetAllModelPath();
}

/// <summary>ゲーム共通基盤で使う Model Info の情報と処理をまとめる型</summary>
struct ModelInfo
{
    std::string name;
    std::string path;
    std::shared_ptr<CFbxMesh> mesh;
    std::shared_ptr<MeshCollider> coll;

    /// ModelInfo を初期化する
    /// @param name 名前
    /// @param path パス
    ModelInfo(const char* name, const char* path)
        : name(name ? name : ""), path(path ? path : ""), mesh(nullptr), coll(nullptr)
    {
    }

    /// ModelInfo を初期化する
    /// @param name 名前
    ModelInfo(const char* name)
        : name(name ? name : ""), path(""), mesh(nullptr), coll(nullptr)
    {
    }
};
