#include "TutorialAnimal.h"

#include "../System/GameInstance.h"
#include "../Player/Player.h"
#include "../Utils/Animator.h"
#include "../Enemies/System/EnemyManager.h"
#include "../Stage/Ground.h"


CTutorialAnimal::CTutorialAnimal(const VECTOR3& pos)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_pMesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);

    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);

    transform.position = pos;
    transform.rotation = VECTOR3(0, XM_PI, 0);

    m_pGround = ObjectManager::FindGameObject<CGround>();
}

void CTutorialAnimal::Update()
{
    CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
    // 蜷ｸ縺・ｾｼ繧遽・峇蜀・°縺､蜷ｸ縺・ｾｼ繧繝懊ち繝ｳ繧呈款縺励※縺・◆繧我ｽ咲ｽｮ繧定ｶｳ縺励※縺・￥ //
    if (pPl->IsWithSuctionCone(transform.position) && pPl->GetIsSuckUp())
    {
        // 蜷ｸ縺・ｾｼ縺ｿ譎ゅ・遘ｻ蜍暮溷ｺｦ菫よ焚 //
        constexpr int SUCTION_SPEED_FACTOR = 1;

        transform.position += pPl->CalcSuctionDisplacement(SUCTION_SPEED_FACTOR, transform.position);
        m_velocityY = 0.0f;  // 蜷ｸ縺・ｾｼ縺ｿ荳ｭ縺ｯ驥榊鴨繧偵Μ繧ｻ繝・ヨ //
    }
    else
    {
        ApplyGravity();
    }
}

////////////////////
// Score繧定ｶｳ縺励※繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒDestroy 
////////////////////
void CTutorialAnimal::Destroy()
{
    // 謐慕佐譎ゅ↓蜉邂励☆繧狗ｵ碁ｨ灘､ //
    constexpr int CAPTURE_EXP = 1;
    // 謐慕佐譎ゅ↓蜉邂励☆繧九せ繧ｳ繧｢ //
    constexpr int CAPTURE_SCORE = 100;
    // 謐慕佐譎ゅ↓蜉邂励☆繧区黒迯ｲ謨ｰ //
    constexpr int CAPTURE_COUNT = 1;
    ObjectManager::FindGameObject<CPlayerLevel>()->AddExp(CAPTURE_EXP);
    ObjectManager::FindGameObject<CGameInstance>()->AddScore(CAPTURE_SCORE);
    ObjectManager::FindGameObject<CGameInstance>()->AddCapture(CAPTURE_COUNT);
    DestroyMe();
}

CTutorialAnimal::~CTutorialAnimal() = default;

////////////////////
// 驥榊鴨繧帝←逕ｨ縺吶ｋ 
////////////////////
void CTutorialAnimal::ApplyGravity()
{
    static constexpr float GRAVITY = 9.8f;
    static constexpr float GROUND_CHECK_OFFSET = 0.1f;
    static constexpr float GROUND_SKIN = 0.02f;

    m_velocityY -= GRAVITY * SceneManager::DeltaTime();
    const float dt = SceneManager::DeltaTime();
    const float nextY = transform.position.y + m_velocityY * dt;

    if (m_pGround != nullptr)
    {
        const float fromY = transform.position.y + GROUND_CHECK_OFFSET;
        const float toY = nextY - GROUND_CHECK_OFFSET;

        if (toY < fromY)
        {
            const VECTOR3 rayStart = VECTOR3(transform.position.x, fromY, transform.position.z);
            const VECTOR3 rayEnd = VECTOR3(transform.position.x, toY, transform.position.z);

            MeshCollider::CollInfo collInfo;
            if (m_pGround->HitLineToMesh(rayStart, rayEnd, &collInfo))
            {
                transform.position.y = collInfo.hitPosition.y + GROUND_SKIN;
                m_velocityY = 0.0f;
                return;
            }
        }
    }

    transform.position.y = nextY;
}

