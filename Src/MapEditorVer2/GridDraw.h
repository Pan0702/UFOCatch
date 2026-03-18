#pragma once
#include "../Common/Object3D.h"

class GridDraw : public Object3D
{
private:
    bool is_grid_draw_ = false;

private:
    void Draw() override;

    /// <summary>XZ蟷ｳ髱｢縺ｫ繧ｰ繝ｪ繝・ラ邱壹ｒ謠冗判縺吶ｋ縲・0繝槭せ縺斐→縺ｫ邱壹ｒ螟ｪ縺剰｡ｨ遉ｺ縺吶ｋ</summary>
    static void Grid();

public:
    /// <summary>繧ｰ繝ｪ繝・ラ謠冗判繝輔Λ繧ｰ縺ｮ迴ｾ蝨ｨ蛟､繧定ｿ斐☆</summary>
    bool GetDrawFlag() const { return is_grid_draw_; }
    void SetDrawFlag(bool flag) { is_grid_draw_ = flag; }
};

