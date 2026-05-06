#pragma once
#include "MouseRay.h"
#include "../Framework/ResourceManager.h"
#include "../Common/Object3D.h"
#include "../Stage/StageCollision.h"

struct StageDataInfo
{
    Transform transform;
    std::string modelName;
    StageColl c;

    StageDataInfo()
    {
        transform.position = VECTOR3(0, 0, 0);
        modelName = "";
    }

    StageDataInfo(const std::string& model_name, const VECTOR3& pos)
    {
        modelName = model_name;
        transform.position = pos;
    }

    StageDataInfo(const std::string& model_name, const Transform& t)
    {
        modelName = std::move(model_name);
        transform = t;
    }
};

class StageData : public Object3D
{
private:
    std::vector<StageDataInfo> m_stageData;
    int m_selectedModel = -1;

private:
    void Draw() override;

public:
    StageData();
    ~StageData() = default;

    /// <summary>指定座標とモデル名でオブジェクトをステージに追加する</summary>
    /// <param name="pos">配置するワールド座標</param>
    /// <param name="modelName">使用するモデルの名前</param>
    void AddModel(const VECTOR3& pos, const std::string& modelName);

    int AddModel(const Transform& t, const std::string& modelName);

    /// <summary>Transform  全体を指定してオブジェクトをステージに追加する（インポート用）</summary>               
    /// <param name="modelName">使用する モデルの名前</param>        
    /// <param name="transform">配置するTransform（位置・回転・スケール）</param>  
    void AddModelWithTransform(const std::string& modelName, const Transform& transform);

    /// <summary>ステージデータをJSONファイルにエクスポートする</summary>
    /// <param name="filename">出力ファイル名（拡張子なし）</param>
    void Export(const std::string& filename) const;

    /// <summary>レイとステージ上の全オブジェクトのメッシュコライダーを判定し、最接近のインデックスを返す</summary>
    /// <param name="ray">判定に使用するレイ</param>
    /// <param name="collOut">衝突情報の出力先</param>
    /// <returns>当たったオブジェクトのインデックス。当たらなければ-1</returns>
    int RayHitTest(const Ray& ray, MeshCollider::CollInfo* collOut) const;

    /// <summary>現在選択中のオブジェクトをステージから削除する</summary>
    void DeleteModel();

    /// <summary>指定インデックスにあるオブジェクトをステージから削除する</summary>
    /// /// <param name="index">指定インデックス</param>
    void DeleteModel(int index);

    void DeleteModel(const std::string& modelName);

    /// <summary>現在選択中のオブジェクトのTransformポインタを返す。未選択時はnullptr</summary>
    Transform* GetSelectedTransform();

    /// <summary>指定インデックスのオブジェクトのTransformを上書きする（Undo/Redo用）</summary>
    /// <param name="index">対象インデックス</param>
    /// <param name="transform">設定するtransform</param>
    void SetSelectedTransform(int index, const Transform& transform);

    /// <summary>選択中のオブジェクトインデックスを設定する</summary>
    /// <param name="index">選択するインデックス</param>
    void SetModel(int index);

    /// <summary>指定インデックスのオブジェクトをステージにコピーする</summary>
    /// <param name="index"> コピーするオブジェクトの配列番号　</param>
    void CopyModel(int index);

    /// <summary>現在選択中のオブジェクトインデックスを返す</summary>
    int GetSelectIndex() const;

    /// <summary>ステージに設置されているすべてのオブジェクトを返す</summary>
    const std::vector<StageDataInfo>& GetStageDataInfo() const;

    Transform GetTrans() const;

    std::string GetModelName() const;
};
