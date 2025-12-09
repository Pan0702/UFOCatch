#include "EnemyBase.h"
#include "../../10_Stage/Ground.h"

namespace 
{
    static constexpr float GROUND_CHECK_OFFSET = 0.1f;
}
CEnemyBase::CEnemyBase()
    : m_velocityY(0.0f), m_pGround(nullptr)
{
    m_pCurrentState = nullptr;
}

void CEnemyBase::SetState(CBaseState::Type type)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_cubeStates[type];
    m_pCurrentState->Enter();
}

CEnemyBase::~CEnemyBase()
{
}

void CEnemyBase::Update()
{
    ApplyGravity();

    if (m_pCurrentState)
    {
        m_pCurrentState->Update();
    }
}

void CEnemyBase::ApplyGravity()
{
    if (!ShouldApplyGravity())
    {
        m_velocityY = 0.0f;
        return;
    }
    static constexpr float GRAVITY = 9.8f;

    static constexpr float GROUND_SKIN = 0.02f; // 少しだけめり込み防止で浮かせる //
    m_velocityY -= GRAVITY * SceneManager::DeltaTime();
    const float dt = SceneManager::DeltaTime();
    float nextY = transform.position.y + m_velocityY * dt;

    if (m_pGround != nullptr)
    {
        // レイを現在位置より少し上から、移動後位置より少し下まで飛ばしてトンネリングを防止 //
        float fromY = transform.position.y + GROUND_CHECK_OFFSET;
        float toY = nextY - GROUND_CHECK_OFFSET;
        // 降下時のみ地面チェック //
        if (toY < fromY)
        {
            VECTOR3 rayStart = VECTOR3(transform.position.x, fromY, transform.position.z);
            VECTOR3 rayEnd = VECTOR3(transform.position.x, toY, transform.position.z);

            MeshCollider::CollInfo collInfo;
            bool hit = m_pGround->HitLineToMesh(rayStart, rayEnd, &collInfo);

            if (hit)
            {
                // 少しだけ浮かせて再衝突を防ぐ //
                transform.position.y = collInfo.hitPosition.y + GROUND_SKIN;
                m_velocityY = 0.0f;
                return;
            }
        }
    }

    transform.position.y = nextY;
}

bool CEnemyBase::IsGrounded() const
{
    if (m_pGround == nullptr) return false;

    VECTOR3 rayStart = transform.position + VECTOR3(0, GROUND_CHECK_OFFSET, 0);
    VECTOR3 rayEnd = transform.position + VECTOR3(0, -GROUND_CHECK_OFFSET, 0);

    MeshCollider::CollInfo collInfo;
    return m_pGround->HitLineToMesh(rayStart, rayEnd, &collInfo);
}
