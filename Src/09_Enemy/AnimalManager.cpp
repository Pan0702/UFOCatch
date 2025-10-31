#include "AnimalManager.h"

#include "ACube.h"
#include "../06_GameLib/BBox.h""
#include "../08_Player/Player.h"
#include <chrono>
#include <thread>

#include "EnemyHuman.h"

CAnimalManager::CAnimalManager()
{
}

CAnimalManager::~CAnimalManager()
{
    if (m_pPlayer)
    {
        m_pPlayer = nullptr;
        delete m_pPlayer;
    }
}

void CAnimalManager::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_0))
    {
        Instantiate<CACube>();
    }
    ObjectManager::FindGameObject<SpatialGrid>()->ClearGrid();
}


void CAnimalManager::Draw()
{
}


VECTOR3 CAnimalManager::GetObjectSize(MeshCollider* meshColl) const
{
    return meshColl->bBox.max;
}
