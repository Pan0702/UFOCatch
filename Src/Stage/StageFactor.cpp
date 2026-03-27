#include "StageFactor.h"

#include "CubeBox.h"
#include "Ground.h"
#include "StageObject.h"
#include "../MapEditor/Import.h"
#include "../Framework/ResourceManager.h"
#include "../Utils/MyLib.h"
CStageFactor::CStageFactor()
{
    Instantiate<CGround>("data/Ground/Prefabs/MapPlane001.mesh",VECTOR3(5.0f,3.0f,5.0f));
    Instantiate<CCubeBox>("data/Ground/CubeBoxSky.mesh");
   // new CStageObject("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(1.0f,0.0f,1.0f),2);
    
}

void CStageFactor::SpawnObjects(float sizeX,float sizeZ,int num)
{
    for (int i = 0; i < num; ++i)
    {
       float randomX = Randomf(-sizeX, sizeX);
        float randomZ = Randomf(-sizeZ, sizeZ);
        Instantiate< CStageObject>("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(randomX,0.0f,randomZ),true);
    }
}

void CStageFactor::SpawnObjects(const std::string& path,const VECTOR2& size, int num)
{
    if (MyLib::IsSameFormat(path,"json"))
    {
        for (int i = 0; i < num; ++i)
        {
            float randomX = Randomf(-size.x, size.x);
            float randomZ = Randomf(-size.y, size.y);
            Instantiate< CStageObject>("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(randomX,0.0f,randomZ),true);
        }
    }
    else
    {
            std::vector<Info> vector = Import::ImportFromFile(path);
        for (auto v : vector)
        {
            // モデルが未ロードの場合、自動的にロードする。
            if (ResourceManager::GetModel(v.modelName.c_str()) == nullptr)
            {
                ResourceManager::LoadFbx(v.modelName.c_str(), v.modelPath.c_str());
            }
            Instantiate<CStageObject>(path.c_str(),v.transform,true);
        }
    }
}
