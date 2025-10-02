#include "AnimalManager.h"

#include "ACube.h"
#include "BBox.h"
#include "Player.h"
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

void CAnimalManager::MoveForUFO(const VECTOR3& animalPos,const VECTOR3& distanceFromObjectToUFO)
{
    if (ObjectManager::FindGameObject<CPlayer>()->GetPos().y >= animalPos.y)  
    {
        transform.position += distanceFromObjectToUFO;
    }
    else
    {
        DestroyMe();
    }
}





