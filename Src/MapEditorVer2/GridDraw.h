#pragma once
#include "../Common/Object3D.h"

class GridDraw : public Object3D
{
private:
    bool is_grid_draw_ = false;

private:
    void Draw() override;

    /// <summary>XZ平面にグリッド線を描画する。10マスごとに線を太く表示する</summary>
    static void Grid();

public:
    /// <summary>グリッド描画フラグの現在値を返す</summary>
    bool GetDrawFlag() const { return is_grid_draw_; }
    void SetDrawFlag(bool flag) { is_grid_draw_ = flag; }
};
