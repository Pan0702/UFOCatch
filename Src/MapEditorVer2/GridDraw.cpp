#include "GridDraw.h"

void GridDraw::Draw()
{
    if (is_grid_draw_)
    {
        Grid();
    }
}

// XZ平面にグリッド線を描画する。10マスごとに線を太く表示する
void GridDraw::Grid()
{
    CSprite spr;
    constexpr float GIRD_SIZE = 500.0f;
    constexpr float MINOR_LINE_ALPHA    = 0.5f;
    constexpr int   MAJOR_GRID_INTERVAL = 10;    // Z 方向の線（X 軸に平行）
    for (int i = -GIRD_SIZE; i <= GIRD_SIZE; i++)
    {
        if (i % MAJOR_GRID_INTERVAL == 0)
        {
            // 主グリッド線（10マスごと）は不透明で描画
            spr.DrawLine3D(VECTOR3(GIRD_SIZE, 0, i), VECTOR3(-GIRD_SIZE, 0, i), RGB(255, 255, 255));
        }
        else
        {
            // 補助グリッド線は半透明で描画
            spr.DrawLine3D(VECTOR3(GIRD_SIZE, 0, i), VECTOR3(-GIRD_SIZE, 0, i), RGB(255, 255, 255), MINOR_LINE_ALPHA);
        }
    }

    // X 方向の線（Z 軸に平行）
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
