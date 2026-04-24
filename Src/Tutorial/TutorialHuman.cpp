#include "TutorialHuman.h"
#include "../System/VisionSystem.h"
#include "../Player/Player.h"
#include "../Player/PlayerHP.h"
#include "../Utils/Animator.h"
#include "../Enemies/System/EnemyManager.h"

namespace
{
    // 初期回転角度（度） //
    constexpr float INITIAL_ROTATION_DEG = 180.0f;
}

CTutorialHuman::CTutorialHuman(const VECTOR3& pos)
{
    transform.position = pos;
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Human/Human.mesh");
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);

    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);

    m_inSight = false;
    transform.rotation.y = INITIAL_ROTATION_DEG * DegToRad;
}


CTutorialHuman::~CTutorialHuman() = default;

void CTutorialHuman::Update()
{
    CVisionSystem* vision = ObjectManager::FindGameObject<CVisionSystem>();
    CPlayer* player = ObjectManager::FindGameObject<CPlayer>();

    vision->SetCircleCenter(player->GetPos());
    
    // 扇形の視界内にプレイヤーがいて、かつ吸い込みボタンが押されているかチェック //
    m_inSight = vision->SectorCircleCollision(ToVec2XZ(transform.position), transform.rotation.y)
        && player->GetIsSuckUp();

    if (m_inSight)
    {
        // 視界内で吸い込み中ならHPを減らす //
        ObjectManager::FindGameObject<CPlayerHP>()->SubHP();
    }
    else
    {
        // 視界外ならフラグをリセット //
        ObjectManager::FindGameObject<CPlayerHP>()->ResetFlag();
    }
}