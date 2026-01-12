#define NOMINMAX
#include "EnemyBase.h"
#include "../../10_Stage/Ground.h"
#include "../../10_Stage/CStageObject.h"
#include "../System/AnimalManager.h"

namespace 
{
    static constexpr float GROUND_CHECK_OFFSET = 0.1f;
}
CEnemyBase::CEnemyBase()
    : m_velocityY(0.0f), m_pGround(nullptr)
{
    m_pCurrentState = nullptr;
    m_pBBox = nullptr;
}

CBBox* CEnemyBase::CreateBBox()
{
    if (m_pBBox == nullptr && m_pMesh != nullptr)
    {
        m_pBBox = new CBBox(m_pMesh->m_vMin, m_pMesh->m_vMax);
    }
    return m_pBBox;
}


void CEnemyBase::SetState(CBaseState::Type type)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_cubeStates[type];
    m_pCurrentState->Enter();
}

CEnemyBase::~CEnemyBase()
{
    SAFE_DELETE(m_pBBox);
    for (auto& state : m_cubeStates)
    {
        SAFE_DELETE(state.second);
    }
}

void CEnemyBase::Update()
{
    if (m_pCurrentState != nullptr && m_pCurrentState == m_cubeStates[CBaseState::Type::DESTROY])
    {
        return;
    }

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

    static constexpr float GROUND_SKIN = 0.02f; // 地面との接触時のめり込み防止用のスキン値 //
    m_velocityY -= GRAVITY * SceneManager::DeltaTime();
    const float dt = SceneManager::DeltaTime();
    const float nextY = transform.position.y + m_velocityY * dt;

    if (m_pGround != nullptr)
    {
        // 現在の位置と次の位置を使って、オフセットを考慮したレイキャストの範囲を決定 //
        const float fromY = transform.position.y + GROUND_CHECK_OFFSET;
        const float toY = nextY - GROUND_CHECK_OFFSET;
        // 下向きの地面チェック //
        if (toY < fromY)
        {
            const VECTOR3 rayStart = VECTOR3(transform.position.x, fromY, transform.position.z);
            const VECTOR3 rayEnd = VECTOR3(transform.position.x, toY, transform.position.z);

            MeshCollider::CollInfo collInfo;
            bool hit = m_pGround->HitLineToMesh(rayStart, rayEnd, &collInfo);

            if (hit)
            {
                // 衝突した場合、地面の位置にスキン値を加えて位置を調整 //
                transform.position.y = collInfo.hitPosition.y + GROUND_SKIN;
                m_velocityY = 0.0f;
                return;
            }
        }
    }

    transform.position.y = nextY;
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

std::vector<CEnemyBase*> CEnemyBase::GetNearbyEnemies() const
{
    // AnimalManagerを取得 //
    CAnimalManager* manager = ObjectManager::FindGameObject<CAnimalManager>();
    if (manager == nullptr)
    {
        return std::vector<CEnemyBase*>();
    }

    // 自分の2D境界ボックス（XZ平面での位置とサイズ）を取得 //
    VECTOR2 pos, size;
    if (!GetBounds2D(pos, size))
    {
        return std::vector<CEnemyBase*>();
    }

    // マネージャーの四分木を使って、周辺のエネミーを効率的に取得 //
    return manager->GetNearbyEnemies(const_cast<CEnemyBase*>(this), pos, size);
}

void CEnemyBase::UpdateBBox()
{
    if (m_pBBox != nullptr)
    {
        m_pBBox->m_mWorld = transform.matrix();
    }
}

void CEnemyBase::ResolveOBBCollisions()
{
    if (m_pBBox == nullptr) return;

    // 四分木から周辺のエネミーを効率的に取得//
    std::vector<CEnemyBase*> nearbyEnemies = GetNearbyEnemies();

    // 周辺エネミーと当たり判定//
    for (auto* enemy : nearbyEnemies)
    {
        if (enemy == this) continue;  // 自分自身はスキップ//
        if (enemy->GetBBox() == nullptr) continue;

        VECTOR3 hitPos, hitNormal;
        if (m_pBBox->OBBCollisionDetection(enemy->GetBBox(), &hitPos, &hitNormal))
        {
            // 衝突した場合、押し戻し処理を実行//
            CalcApplyPushback(enemy);
        }
    }
}

void CEnemyBase::CalcApplyPushback(CEnemyBase* other)
{
    if (m_pBBox == nullptr || other == nullptr || other->GetBBox() == nullptr) return;

    // 自分と相手のOBB中心座標を計算//
    MATRIX4X4 myCenterMat = XMMatrixTranslation(
        m_pBBox->m_fLengthX + m_pBBox->m_vMin.x,
        m_pBBox->m_fLengthY + m_pBBox->m_vMin.y,
        m_pBBox->m_fLengthZ + m_pBBox->m_vMin.z
    );
    // ワールド空間に変換//
    myCenterMat = myCenterMat * m_pBBox->m_mWorld;
    // 行列から位置成分を抽出して自分の中心座標を取得//
    VECTOR3 myCenter =GetPositionVector(myCenterMat);

    // 相手のOBB中心座標を計算//
    CBBox* otherBBox = other->GetBBox();
    MATRIX4X4 otherCenterMat = XMMatrixTranslation(
        otherBBox->m_fLengthX + otherBBox->m_vMin.x,
        otherBBox->m_fLengthY + otherBBox->m_vMin.y,
        otherBBox->m_fLengthZ + otherBBox->m_vMin.z
    );
    // ワールド空間に変換//
    otherCenterMat = otherCenterMat * otherBBox->m_mWorld;
    // 行列から位置成分を抽出して相手の中心座標を取得//
    VECTOR3 otherCenter =  GetPositionVector(otherCenterMat);

    // 押し戻しベクトルを計算//
    VECTOR3 pushDirection = myCenter - otherCenter;
    pushDirection.y = 0.0f;  // Y成分を無視してXZ平面のみで押し戻す//
    float distance = magnitude(pushDirection);

    if (distance > 0.001f)  // 0除算をnormで起こさないために値がい小さい場合はスキップ//
    {
        pushDirection = normalize(pushDirection);

        static constexpr float PUSHBACK_SPEED = 6.0f;  // 1秒あたりの押し戻し速度

        // 押し戻し距離//
        float pushDistance = PUSHBACK_SPEED * SceneManager::DeltaTime();

        // 位置を更新//
        transform.position += pushDirection * pushDistance;
    }
}

//------------------------------------------------------------------------
// ステージオブジェクトとの衝突判定と押し戻し処理
//------------------------------------------------------------------------
void CEnemyBase::ResolveStageCollisions()
{
    if (m_pBBox == nullptr) return;

    // シーン内の全てのCStageObjectを取得
    std::list<CStageObject*> stageObjects = ObjectManager::FindGameObjects<CStageObject>();

    // 各ステージオブジェクトと衝突判定
    for (CStageObject* stage : stageObjects)
    {
        if (stage == nullptr) continue;
        stage->ResolveEnemyCollision(this);
    }
}

