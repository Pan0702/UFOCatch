#define NOMINMAX
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

    static constexpr float GROUND_SKIN = 0.02f; // ?????????????h?~????????? //
    m_velocityY -= GRAVITY * SceneManager::DeltaTime();
    const float dt = SceneManager::DeltaTime();
    const float nextY = transform.position.y + m_velocityY * dt;

    if (m_pGround != nullptr)
    {
        // ???C???????u??????�H??A??????u???????????????g???l?????O??h?~ //
        const float fromY = transform.position.y + GROUND_CHECK_OFFSET;
        const float toY = nextY - GROUND_CHECK_OFFSET;
        // ?~???????n??`?F?b?N //
        if (toY < fromY)
        {
            const VECTOR3 rayStart = VECTOR3(transform.position.x, fromY, transform.position.z);
            const VECTOR3 rayEnd = VECTOR3(transform.position.x, toY, transform.position.z);

            MeshCollider::CollInfo collInfo;
            bool hit = m_pGround->HitLineToMesh(rayStart, rayEnd, &collInfo);

            if (hit)
            {
                // ????????????????????h?? //
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

bool CEnemyBase::GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const
{
    if (m_pMesh == nullptr)
    {
        //Meshがなければ終わる//
        return false;
    }
    
    // XZ平面での4頂点を取得//
    const VECTOR3 min = m_pMesh->m_vMin;
    const VECTOR3 max = m_pMesh->m_vMax;
    
    VECTOR3 corners[4] = {
        VECTOR3(min.x, 0, min.z),
        VECTOR3(max.x, 0, min.z),
        VECTOR3(min.x, 0, max.z),
        VECTOR3(max.x, 0, max.z)
    };

    // 回転行列を取得//
    MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y);
    MATRIX4X4 scaleM = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
    MATRIX4X4 transformMatrix = scaleM * rotY;

    // 変換後の頂点の最小・最大値を求める //
    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minZ = FLT_MAX, maxZ = -FLT_MAX;

    for (const auto& corner : corners)
    {
        VECTOR3 transformed = XMVector3TransformCoord(corner, transformMatrix);
        minX = std::min(minX, transformed.x);
        maxX = std::max(maxX, transformed.x);
        minZ = std::min(minZ, transformed.z);
        maxZ = std::max(maxZ, transformed.z);
    }

    // ワールド座標に変換
    outPos = VECTOR2(
        (minX + maxX) * 0.5f + transform.position.x,
        (minZ + maxZ) * 0.5f + transform.position.z
    );
    outSize = VECTOR2(maxX - minX, maxZ - minZ);
    
    return true;
}
