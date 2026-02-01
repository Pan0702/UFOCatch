#include "Flog.h"

void Flog::RemoveFromArray(CSheep* sheep)
{
    sheeps.erase(std::remove(sheeps.begin(), sheeps.end(), sheep), sheeps.end());
}

FlogInfo Flog::CalcFlogInfo() const
{
    FlogInfo info;
    info.centroid = VECTOR3(0,0,0);
    info.maxDistance = 0.0f;
    info.furthestSheep = nullptr;
    
    if (sheeps.empty())return info;
    for (auto sheep : sheeps)   
    {  
        info.centroid += sheep->GetTransform().position;
    }
    info.centroid /= sheeps.size();
    
    for (auto sheep : sheeps)
    {
        float distance = (info.centroid - sheep->GetTransform().position).Length();
        if (distance > info.maxDistance)
        {
            info.maxDistance = distance;
            info.furthestSheep = sheep;
        }
    }
    return info;
}
