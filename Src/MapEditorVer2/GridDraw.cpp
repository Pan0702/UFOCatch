#include "GridDraw.h"

namespace
{
    // 10マスごとにグリッド線を太く（不透明に）表示する
    constexpr int   kMajorGridInterval = 10;

    // 細いグリッド線のアルファ値（0〜1）
    constexpr float kMinorLineAlpha    = 0.5f;
}

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
    constexpr float gridSize = 500.0f;

    // Z 方向の線（X 軸に平行）
    for (int i = -gridSize; i <= gridSize; i++)
    {
        if (i % kMajorGridInterval == 0)
        {
            // 主グリッド線（10マスごと）は不透明で描画
            spr.DrawLine3D(VECTOR3(gridSize, 0, i), VECTOR3(-gridSize, 0, i), RGB(255, 255, 255));
        }
        else
        {
            // 補助グリッド線は半透明で描画
            spr.DrawLine3D(VECTOR3(gridSize, 0, i), VECTOR3(-gridSize, 0, i), RGB(255, 255, 255), kMinorLineAlpha);
        }
    }

    // X 方向の線（Z 軸に平行）
    for (int i = -gridSize; i <= gridSize; i++)
    {
        if (i % kMajorGridInterval == 0)
        {
            spr.DrawLine3D(VECTOR3(i, 0, gridSize), VECTOR3(i, 0, -gridSize), RGB(255, 255, 255));
        }
        else
        {
            spr.DrawLine3D(VECTOR3(i, 0, gridSize), VECTOR3(i, 0, -gridSize), RGB(255, 255, 255), kMinorLineAlpha);
        }
    }
}
