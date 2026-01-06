#include "Tutorial.h"

#include "../GameInstance.h"
#include "../08_Player/Player.h"
#include "10_Stage/Ground.h"

CTutorial::CTutorial()
{
}

CTutorial::~CTutorial()
{
}

void CTutorial::Update()
{
    \
    Object3D::Update();
    if (ObjectManager::FindGameObject<CPlayer>()->IsWithSuctionCone(transform.position))
    {
        
    }
    CGameInstance* pGI = ObjectManager::FindGameObject<CGameInstance>();
    if (pGI->GetScore() > 0)
    {
        
    }
}
