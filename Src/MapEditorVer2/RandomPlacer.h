#pragma once
#include "UndoManager.h"
#include "../Object3D.h"
#include "TRSObject/TRS.h"

class RandomPlacer : public Object3D
{
private:
    VECTOR3 point_max_;
    VECTOR3 point_min_;  

    // ImGuiパラメータ
    int              count_        = 10;
    float            height_       = 1.0f;
    std::string      model_name_;   // 配置するモデル名
    bool             is_draw_      = false;
    std::vector<std::string> model_names_;

    StageData*       stage_data_   = nullptr;
    UndoManager*     undo_manager_ = nullptr;
    TRS*             trs_ = nullptr;
private:
    void Update() override;
    void SpawnObject(int model, bool flag);

public:
    RandomPlacer();
    Transform* GetTransform();
    ~RandomPlacer() = default;
    void Draw() override;
    void SetDrawFlag(bool flag) { is_draw_ = flag; }
    
};
