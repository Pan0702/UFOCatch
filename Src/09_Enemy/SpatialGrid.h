#pragma once
#include <unordered_map>

#include "ACube.h"
#include "../06_GameLib/MyMath.h"

class CACube;
class SpatialGrid
{
public:
    SpatialGrid();
    ~SpatialGrid();
    void Insert(CACube* cube);
    std::vector<CACube*> CheckNearby(CACube* cube);
    void ClearGrid();
    

private:
    std::string CalcGridPos(const VECTOR3& pos);
    
    std::unordered_map<std::string, std::vector<CACube*>> m_grid;
    
};
