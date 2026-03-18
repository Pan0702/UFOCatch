#include "GridDraw.h"

void GridDraw::Draw()
{
    if (is_grid_draw_)
    {
        Grid();
    }
}

// XZ蟷ｳ髱｢縺ｫ繧ｰ繝ｪ繝・ラ邱壹ｒ謠冗判縺吶ｋ縲・0繝槭せ縺斐→縺ｫ邱壹ｒ螟ｪ縺剰｡ｨ遉ｺ縺吶ｋ
void GridDraw::Grid()
{
    CSprite spr;
    constexpr float GIRD_SIZE = 500.0f;
    constexpr float MINOR_LINE_ALPHA    = 0.5f;
    constexpr int   MAJOR_GRID_INTERVAL = 10;    // Z 譁ｹ蜷代・邱夲ｼ・ 霆ｸ縺ｫ蟷ｳ陦鯉ｼ・
    for (int i = -GIRD_SIZE; i <= GIRD_SIZE; i++)
    {
        if (i % MAJOR_GRID_INTERVAL == 0)
        {
            // 荳ｻ繧ｰ繝ｪ繝・ラ邱夲ｼ・0繝槭せ縺斐→・峨・荳埼乗・縺ｧ謠冗判
            spr.DrawLine3D(VECTOR3(GIRD_SIZE, 0, i), VECTOR3(-GIRD_SIZE, 0, i), RGB(255, 255, 255));
        }
        else
        {
            // 陬懷勧繧ｰ繝ｪ繝・ラ邱壹・蜊企乗・縺ｧ謠冗判
            spr.DrawLine3D(VECTOR3(GIRD_SIZE, 0, i), VECTOR3(-GIRD_SIZE, 0, i), RGB(255, 255, 255), MINOR_LINE_ALPHA);
        }
    }

    // X 譁ｹ蜷代・邱夲ｼ・ 霆ｸ縺ｫ蟷ｳ陦鯉ｼ・
    for (int i = -GIRD_SIZE; i <= GIRD_SIZE; i++)
    {
        if (i % MAJOR_GRID_INTERVAL == 0)
        {
            spr.DrawLine3D(VECTOR3(i, 0, GIRD_SIZE), VECTOR3(i, 0, -GIRD_SIZE), RGB(255, 255, 255));
        }
        else
        {
            
            spr.DrawLine3D(VECTOR3(i, 0, GIRD_SIZE), VECTOR3(i, 0, -GIRD_SIZE), RGB(255, 255, 255), MINOR_LINE_ALPHA);
        }
    }
}

