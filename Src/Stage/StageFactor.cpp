#include "StageFactor.h"

#include "CubeBox.h"
#include "Ground.h"
#include "SkyBox.h"
#include "StageObject.h"
#include "StageQuadTree.h"
#include "../Common/ShadowObject.h"
#include "../MapEditor/Import.h"
#include "../Framework/ResourceManager.h"
#include "../Utils/MyLib.h"
#include "StageCollision.h"
using namespace Constants;

namespace
{
    void AttachShadowIfStageObstacle(CStageObject* object, const StageColl& coll)
    {
        if (object != nullptr && coll.useOBB && !coll.useHitGround)
        {
            Instantiate<CShadowObject>(object, TEXT("data/CircleSuction.png"));
        }
    }
}

CStageFactor::CStageFactor()
{
    Instantiate<CCubeBox>(Model::BACK_DROP);
    // new CStageObject("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(1.0f,0.0f,1.0f),2);
}

CStageFactor::CStageFactor(const char* path)
{
    Instantiate<CSkyBox>(Model::BACK_DROP);
    SpawnObjects(path);
}

void CStageFactor::SpawnObjects(float sizeX, float sizeZ, int num)
{
    for (int i = 0; i < num; ++i)
    {
        float randomX = Randomf(-sizeX, sizeX);
        float randomZ = Randomf(-sizeZ, sizeZ);
        StageColl coll;
        coll.useOBB = true;
        coll.useHitGround = false;
        CStageObject* object = Instantiate<CStageObject>(
            "data/Ground/Prefabs/Tree1a.mesh", VECTOR3(randomX, 0.0f, randomZ), 1.0f, coll);
        AttachShadowIfStageObstacle(object, coll);
    }
}

void CStageFactor::SpawnObjects(const std::string& path, const VECTOR2& size, int num)
{
    if (MyLib::IsSameFormat(path, "json"))
    {
        std::vector<Info> vector = Import::StageInfo(path);
        for (auto v : vector)
        {
            // モデルが未ロードの場合、自動的にロードする。
            if (ResourceManager::GetModel(v.modelName.c_str()) == nullptr)
            {
                ResourceManager::LoadFbx(v.modelName.c_str(), v.modelPath.c_str());
            }
            CStageObject* object = Instantiate<CStageObject>(v.modelPath.c_str(), v.transform, v.soc);
            AttachShadowIfStageObstacle(object, v.soc);
        }
    }
    else
    {
        constexpr int TREE_NUM = 80;
        for (int i = 0; i < TREE_NUM; ++i)
        {
            float randomX = Randomf(-size.x, size.x);
            float randomZ = Randomf(-size.y, size.y);
            StageColl coll;
            coll.useOBB = true;
            coll.useHitGround = false;
            CStageObject* object = Instantiate<CStageObject>(
                "data/Ground/Prefabs/Tree1a.mesh", VECTOR3(randomX, 0.0f, randomZ), 1.0f, coll);
            AttachShadowIfStageObstacle(object, coll);
        }
        StageColl soc;
        soc.useOBB = false;
        soc.useHitGround = true;
        Instantiate<CStageObject>(Model::GROUND, VECTOR3(5.0f, 3.0f, 5.0f), 1.0f, soc);
    }
}
