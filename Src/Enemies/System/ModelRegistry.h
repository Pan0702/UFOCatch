#pragma once
#include <string>
#include <list>

#include "../../Common/Object3D.h"

/// <summary>敵AIで使う Fbx Mesh の情報と処理をまとめる型</summary>
class CFbxMesh;

// アニメーションタイチE
enum AnimationType
{
    A_IDEL = 0,
    A_WALK,
    A_RUN,
    A_SEACH
};

/// <summary>敵AIで使う Model Registry の情報と処理をまとめる型</summary>
class CModelRegistry : public Object3D
{
public:
    /// CModelRegistry を初期化する
    CModelRegistry();
    /// CModelRegistry の終了処理を行う
    ~CModelRegistry();

    /// Mesh を取得する
    /// @param name 名前
    /// @return 対象のポインタ
    CFbxMesh* GetMesh(const std::string& name) const;

    /// Register の処理を行う
    /// @param name 名前
    /// @param mesh mesh に渡す値
    void Register(const std::string& name, CFbxMesh* mesh);

private:
    /// <summary>敵AIで使う Model Entry の情報と処理をまとめる型</summary>
    struct ModelEntry
    {
        std::string name;
        CFbxMesh* mesh;
    };

    std::list<ModelEntry> m_models;

    /// Default Models を読み込む
    void LoadDefaultModels();
};
