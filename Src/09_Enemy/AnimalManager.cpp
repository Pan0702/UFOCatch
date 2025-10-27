#include "AnimalManager.h"

#include "ACube.h"
#include "../06_GameLib/BBox.h"
#include "../08_Player/Player.h"
#include <thread>
#include "ACube.h"
#include "../04_FrameWork/PoolAllocator.h"

namespace
{
    

}

CAnimalManager::CAnimalManager()
{
}

CAnimalManager::~CAnimalManager()
{
    for (int i = 0; i < 100; i++)
    {
        if (pCubes[i] != nullptr)
        {
            Cube.Free(pCubes[i]);
        }
    }
}

void CAnimalManager::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_0))
    {
        // 空いているスロットを探して1個生成
        for (int i = 0; i < 100; i++)
        {
            if (pCubes[i] != nullptr)continue;
            pCubes[i] = Cube.Alloc();
            new(pCubes[i]) CACube(); // placement newの正しい構文
            break; // 1個生成したら終了
        }
    }
}

void CAnimalManager::Draw()
{
}


VECTOR3 CAnimalManager::GetObjectSize(MeshCollider* meshColl) const
{
    return meshColl->bBox.max;
}
