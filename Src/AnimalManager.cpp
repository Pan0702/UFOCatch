#include "AnimalManager.h"

#include "ACube.h"
#include "BBox.h"
#include "Player.h"

CAnimalManager::CAnimalManager()
{
    
}

CAnimalManager::~CAnimalManager()
{
}

void CAnimalManager::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_0))
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



