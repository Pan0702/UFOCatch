#include "AnimalManager.h"

#include "ACube.h"
#include "../06_GameLib/BBox.h"
#include "../08_Player/Player.h"
#include <thread>
#include "ACube.h"
#include "../04_FrameWork/PoolAllocator.h"


CAnimalManager::CAnimalManager()
{
    m_pCubes.resize(100);
    for (int i = 0; i < 100; i++)
    {
        m_pCubes[i] = nullptr;
    }
}

CAnimalManager::~CAnimalManager()
{

}

void CAnimalManager::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_0))
    {
        for (CACube*& cube : m_pCubes)
        {
            if (cube)continue;
            cube = Cube.Alloc();
            new (cube) CACube();
            break; 
        }
    }
    
}

void CAnimalManager::Draw()
{
    
}


bool CAnimalManager::CubesFree()
{
    for (int i = 0; i < 100; i++)
    {
        if (m_pCubes[i] != nullptr)
        {
            Cube.Free(m_pCubes[i]);
            m_pCubes[i] = nullptr;
        }
    }
    return true;
}

VECTOR3 CAnimalManager::GetObjectSize(MeshCollider* meshColl) const
{
    return meshColl->bBox.max;
}
