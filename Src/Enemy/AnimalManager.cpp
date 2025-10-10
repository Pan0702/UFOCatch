#include "AnimalManager.h"

#include "ACube.h"
#include "../GameLib/BBox.h""
#include "../Player/Player.h"
#include <chrono>
#include <thread>

CAnimalManager::CAnimalManager()
{
    
}

CAnimalManager::~CAnimalManager()
{
}

void CAnimalManager::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_0))
    {
        Instantiate<CACube>();
    }
}

void CAnimalManager::Draw()
{

}


VECTOR3 CAnimalManager::GetObjectSize(MeshCollider* meshColl) const
{
    return meshColl->bBox.max;
}






