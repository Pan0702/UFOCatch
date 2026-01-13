#include "TutorialHuman.h"
#include "../11_GameSystem/VisionSystem.h"
#include "../08_Player/Player.h"
#include "../08_Player/PHP.h"
#include "../06_GameLib/Animator.h"
#include "../09_Enemy/System/AnimalManager.h"

CTutorialHuman::CTutorialHuman(const VECTOR3& pos)
{
    transform.position = pos;
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Human/Human.mesh");
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);

    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);

    m_inSight = false;
    transform.rotation.y = 180 * DegToRad;
}


CTutorialHuman::~CTutorialHuman()
{
}

void CTutorialHuman::Update()
{
    CVisionSystem* vision = ObjectManager::FindGameObject<CVisionSystem>();
    CPlayer* player = ObjectManager::FindGameObject<CPlayer>();

    vision->GetCircleInfo().SetCenter(player->GetPos());
    // 扇形の視界内にプレイヤーがいて、かつ吸い込みボタンが押されているかチェック
    m_inSight = vision->SectorCircleCollision(ToVec2XZ(transform.position), transform.rotation.y)
        && player->GetIsSuckUp();

    if (m_inSight)
    {
        ObjectManager::FindGameObject<CPlayerHP>()->SubHP();
        
    }
    else
    {
        ObjectManager::FindGameObject<CPlayerHP>()->ResetFlag();
    }
}
