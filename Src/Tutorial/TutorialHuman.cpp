#include "TutorialHuman.h"
#include "../System/VisionSystem.h"
#include "../Player/Player.h"
#include "../Player/PlayerHP.h"
#include "../Utils/Animator.h"
#include "../Enemies/System/EnemyManager.h"

namespace
{
    // 蛻晄悄蝗櫁ｻ｢隗貞ｺｦ・亥ｺｦ・・//
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
    // 謇・ｽ｢縺ｮ隕也阜蜀・↓繝励Ξ繧､繝､繝ｼ縺後＞縺ｦ縲√°縺､蜷ｸ縺・ｾｼ縺ｿ繝懊ち繝ｳ縺梧款縺輔ｌ縺ｦ縺・ｋ縺九メ繧ｧ繝・け //
    m_inSight = vision->SectorCircleCollision(ToVec2XZ(transform.position), transform.rotation.y)
        && player->GetIsSuckUp();

    if (m_inSight)
    {
        // 隕也阜蜀・〒蜷ｸ縺・ｾｼ縺ｿ荳ｭ縺ｪ繧羽P繧呈ｸ帙ｉ縺・//
        ObjectManager::FindGameObject<CPlayerHP>()->SubHP();
    }
    else
    {
        // 隕也阜螟悶↑繧峨ヵ繝ｩ繧ｰ繧偵Μ繧ｻ繝・ヨ //
        ObjectManager::FindGameObject<CPlayerHP>()->ResetFlag();
    }
}

