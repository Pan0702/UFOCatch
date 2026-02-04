#pragma once
#include <string>
#include <list>

class CFbxMesh;

// アニメーションタイプ
enum AnimationType
{
    A_IDEL = 0,
    A_WALK,
    A_RUN,
    A_SEACH
};

// モデル登録・取得クラス
class CModelRegistry
{
public:
    CModelRegistry();
    ~CModelRegistry();

    // メッシュを名前で取得
    CFbxMesh* GetMesh(const std::string& name) const;

    // メッシュを追加
    void Register(const std::string& name, CFbxMesh* mesh);

private:
    struct ModelEntry
    {
        std::string name;
        CFbxMesh* mesh;
    };

    std::list<ModelEntry> m_models;

    // デフォルトのメッシュをロード
    void LoadDefaultModels();
};
