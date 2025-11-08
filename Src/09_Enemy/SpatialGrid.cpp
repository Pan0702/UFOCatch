#include "SpatialGrid.h"

#include <string>
#include <unordered_set>

namespace
{
    const float CELL_SIZE = 2.0f;
    const int SHIFT = 10;
}

SpatialGrid::SpatialGrid()
{
}

SpatialGrid::~SpatialGrid()
{
}

std::string SpatialGrid::CalcGridPos(const VECTOR3& pos)
{
    int x = std::floor((pos.x + SHIFT) / CELL_SIZE);
    int z = std::floor((pos.z + SHIFT) / CELL_SIZE);
    return std::to_string(x) + "," + std::to_string(z);
}

void SpatialGrid::ClearGrid()
{
    m_grid.clear();   
}

void SpatialGrid::Insert(CACube* cube)
{
    std::string cellPos = CalcGridPos(cube->GetTransform().position);
    m_grid[cellPos].push_back(cube);
}

std::vector<CACube*> SpatialGrid::CheckNearby(CACube* cube)
{
    std::unordered_set<CACube*> nearby;
    int x = std::floor((cube->GetTransform().position.x + SHIFT) / CELL_SIZE);
    int z = std::floor((cube->GetTransform().position.z + SHIFT) / CELL_SIZE);
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = z - 1; j <= z + 1; j++)
        {
            std::string cellPos = std::to_string(i) + "," + std::to_string(j);
            if (m_grid.find(cellPos) == m_grid.end())continue;
            for (auto& c : m_grid[cellPos])
            {
                if (c == cube)continue;
                nearby.insert(c);
            }
        }
    }
    return std::vector<CACube*>(nearby.begin(), nearby.end());
}
