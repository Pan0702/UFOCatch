#pragma once
#include "UndoManager.h"
#include "../Common/Object3D.h"
#include "TRSObject/TRS.h"

/// <summary>ステージエディタで使う Random Placer の情報と処理をまとめる型</summary>
class RandomPlacer : public Object3D
{
private:
    VECTOR3 point_max_ = {};
    VECTOR3 point_min_ = {};

    // ImGuiパラメータ
    int count_ = 10;
    float height_ = 1.0f;
    std::string model_name_; // 配置するモデル名
    bool is_draw_ = false;
    std::vector<std::string> model_names_;

    StageData* stage_data_ = nullptr;
    CUndoManager* undo_manager_ = nullptr;
    TRS* trs_ = nullptr;

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Spawn Object の処理を行う
    /// @param model model に渡す値
    /// @param flag flag に渡す値
    void SpawnObject(int model, bool flag);

public:
    /// RandomPlacer を初期化する
    RandomPlacer();
    /// RandomPlacer の終了処理を行う
    ~RandomPlacer() = default;
    /// 描画する
    void Draw() override;
    /// Transform を取得する
    /// @return 対象のポインタ
    Transform* GetTransform();

    /// Panel を描画する
    void DrawPanel();

    /// Draw Flag を設定する
    /// @param flag flag に渡す値
    void SetDrawFlag(bool flag) { is_draw_ = flag; }
};

/// <summary>ステージエディタで使う Aabb の情報と処理をまとめる型</summary>
struct Aabb
{
    VECTOR3 min, max;

    // 相手のAabbと重なっているか判定
    /// Intersects を返す
    /// @param other other に渡す値
    /// @return 成功または条件を満たす場合 true
    bool Intersects(const Aabb& other) const
    {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
            (min.y <= other.max.y && max.y >= other.min.y) &&
            (min.z <= other.max.z && max.z >= other.min.z);
    }
};
