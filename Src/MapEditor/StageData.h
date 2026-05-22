#pragma once
#include "MouseRay.h"
#include "../Framework/ResourceManager.h"
#include "../Common/Object3D.h"
#include "../Stage/StageCollision.h"

/// <summary>ステージエディタで使う Stage Data Info の情報と処理をまとめる型</summary>
struct StageDataInfo
{
    Transform transform;
    std::string modelName;
    StageColl c;

    /// StageDataInfo を初期化する
    StageDataInfo()
    {
        transform.position = VECTOR3(0, 0, 0);
        modelName = "";
        c = StageColl();
    }

    /// StageDataInfo を初期化する
    /// @param model_name 名前
    /// @param pos 座標
    StageDataInfo(const std::string& model_name, const VECTOR3& pos)
    {
        modelName = model_name;
        transform.position = pos;
        c = StageColl();
    }

    /// StageDataInfo を初期化する
    /// @param model_name 名前
    /// @param t t に渡す値
    StageDataInfo(const std::string& model_name, const Transform& t)
    {
        modelName = std::move(model_name);
        transform = t;
        c = StageColl();
    }
};

/// <summary>ステージエディタで使う Stage Data の情報と処理をまとめる型</summary>
class StageData : public Object3D
{
private:
    std::vector<StageDataInfo> m_stageData;
    int m_selectedModel = -1;

private:
    /// 描画する
    void Draw() override;

public:
    /// StageData を初期化する
    StageData();
    /// StageData の終了処理を行う
    ~StageData() = default;

    /// Model を追加する
    /// @param pos 座標
    /// @param modelName モデル名
    void AddModel(const VECTOR3& pos, const std::string& modelName);

    /// Model を追加する
    /// @param t t に渡す値
    /// @param modelName モデル名
    /// @return 処理結果の数値
    int AddModel(const Transform& t, const std::string& modelName);

    /// Model With Transform を追加する
    /// @param modelName モデル名
    /// @param transform transform に渡す値
    /// @param coll coll に渡す値
    void AddModelWithTransform(const std::string& modelName, const Transform& transform, const StageColl& coll);

    /// 書き出す
    /// @param filename ファイル名
    void Export(const std::string& filename) const;

    /// Ray Hit Test を返す
    /// @param ray 判定に使用するレイ
    /// @param collOut 衝突情報の出力先
    /// @return 処理結果の数値
    int RayHitTest(const Ray& ray, MeshCollider::CollInfo* collOut) const;

    /// Model を削除する
    void DeleteModel();

    /// Model を削除する
    /// @param index インデックス
    void DeleteModel(int index);

    /// Model を削除する
    /// @param modelName モデル名
    void DeleteModel(const std::string& modelName);

    /// Selected Transform を取得する
    /// @return 対象のポインタ
    Transform* GetSelectedTransform();

    /// Selected Transform を設定する
    /// @param index インデックス
    /// @param transform transform に渡す値
    void SetSelectedTransform(int index, const Transform& transform);

    /// Model を設定する
    /// @param index インデックス
    void SetModel(int index);

    /// Copy Model の処理を行う
    /// @param index インデックス
    void CopyModel(int index);

    /// Select Index を取得する
    /// @return 処理結果の数値
    int GetSelectIndex() const;

    /// Stage Data Info を取得する
    /// @return 取得した要素一覧
    const std::vector<StageDataInfo>& GetStageDataInfo() const;

    /// Coll を設定する
    /// @param coll coll に渡す値
    void SetColl(const StageColl& coll);

    /// Coll を取得する
    /// @return 処理結果
    StageColl GetColl() const;
    /// Trans を取得する
    /// @return 処理結果
    Transform GetTrans() const;

    /// Model Name を取得する
    /// @return 取得した文字列
    std::string GetModelName() const;
};
