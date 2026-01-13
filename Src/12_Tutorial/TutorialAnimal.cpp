#include "TutorialAnimal.h"

#include "../GameInstance.h"
#include "../08_Player/Player.h"
#include "../06_GameLib/Animator.h"
#include "../09_Enemy/System/AnimalManager.h"

CTutorialAnimal::CTutorialAnimal(const VECTOR3& pos)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);

    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);

    transform.position = pos;
    transform.rotation = VECTOR3(0, XM_PI, 0);
    
}

void CTutorialAnimal::Update()
{
    CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
    // 吸い込む範囲内かつ吸い込むボタンを押していたら位置を足していく//
    if (pPl->IsWithSuctionCone(transform.position) && pPl->GetIsSuckUp())
    {
        transform.position += pPl->CalcSuctionDisplacement(1, transform.position);
    }
}

void CTutorialAnimal::Destroy()
{
    ObjectManager::FindGameObject<CPlayer>()->AddExp(1);
    // Score加算//
    ObjectManager::FindGameObject<CGameInstance>()->AddScore(1);
    
    DestroyMe();
}

CTutorialAnimal::~CTutorialAnimal() = default;
