#include "StageObject.h"
#include "../09_Enemy/Base/EnemyBase.h"
#include "../04_FrameWork/ResourceManager.h"

//------------------------------------------------------------------------
// コンストラクタ
//
//  const char* meshPath   メッシュファイルのパス
//  bool useOBB            OBBを使用するか
//------------------------------------------------------------------------
CStageObject::CStageObject(const char* meshPath, const VECTOR3& pos, bool useOBB)
{
    m_bUseOBB = useOBB;
    m_pOBB = nullptr;

    // ResourceManagerからメッシュを取得（キャッシュされる）
    m_pMesh = ResourceManager::LoadFbx(meshPath);

    // OBBの作成
    if (m_bUseOBB)
    {
        // メッシュから直接バウンディングボックスを取得
        VECTOR3 vMin = m_pMesh->m_vMin;
        VECTOR3 vMax = m_pMesh->m_vMax;

        // OBBを作成
        m_pOBB = new CBBox(vMin, vMax);
    }

    // 初期位置
    transform.position = VECTOR3(0, 0, 0);
    transform.rotation = VECTOR3(0, 0, 0);
    transform.scale = VECTOR3(1, 1, 1);
}

//------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------
CStageObject::~CStageObject()
{
    if (m_pOBB)
    {
        delete m_pOBB;
        m_pOBB = nullptr;
    }
}

//------------------------------------------------------------------------
// 更新処理
//------------------------------------------------------------------------
void CStageObject::Update()
{
    Object3D::Update();

    // OBBのワールド行列を更新
    if (m_pOBB)
    {
        m_pOBB->m_mWorld = transform.matrix();
    }
}

//------------------------------------------------------------------------
// 描画処理
//------------------------------------------------------------------------
void CStageObject::Draw()
{
    if (m_pMesh)
    {
        m_pMesh->Render(m_pAnimator, transform.matrix());
    }

    // デバッグ用: OBBを描画（必要に応じてコメントアウト）
    if (m_pOBB)
    {
        m_pOBB->Render();
    }
}

//------------------------------------------------------------------------
// OBBとの衝突判定
//
//  CBBox* other           相手のOBB
//  VECTOR3* vHit          衝突位置（Out）
//  VECTOR3* vNormal       衝突法線（Out）※XZ平面のみ（Y成分は0）
//
//  戻り値: 衝突していたらtrue
//------------------------------------------------------------------------
bool CStageObject::HitOBB(CBBox* other, VECTOR3* vHit, VECTOR3* vNormal)
{
    if (!m_pOBB || !other || !m_bUseOBB)
    {
        return false;
    }

    VECTOR3 vHitTemp, vNormalTemp;
    bool bHit = m_pOBB->OBBCollisionDetection(other, &vHitTemp, &vNormalTemp);

    // 結果を格納
    if (bHit)
    {
        if (vHit) *vHit = vHitTemp;

        if (vNormal)
        {
            // XZ平面のみで押し戻し（Y軸を含めると下にめり込む）
            vNormalTemp.y = 0.0f;
            *vNormal = XMVector3Normalize(vNormalTemp);
        }
    }

    return bHit;
}

//------------------------------------------------------------------------
// エネミーとの衝突を解消する
//------------------------------------------------------------------------
void CStageObject::ResolveEnemyCollision(CEnemyBase* pEnemy)
{
    if (!m_pOBB || !pEnemy || !pEnemy->GetBBox()) return;

    VECTOR3 hitPos, hitNormal;
    if (HitOBB(pEnemy->GetBBox(), &hitPos, &hitNormal))
    {
        // 衝突している場合、エネミーを押し戻す
        // ステージオブジェクトの中心からエネミーの中心へのベクトルを計算
        
        // ステージの中心
        MATRIX4X4 stageCenterMat = XMMatrixTranslation(
            m_pOBB->m_fLengthX + m_pOBB->m_vMin.x,
            m_pOBB->m_fLengthY + m_pOBB->m_vMin.y,
            m_pOBB->m_fLengthZ + m_pOBB->m_vMin.z
        );
        stageCenterMat = stageCenterMat * m_pOBB->m_mWorld;
        VECTOR3 stageCenter = GetPositionVector(stageCenterMat);

        // エネミーの中心
        CBBox* enemyBBox = pEnemy->GetBBox();
        MATRIX4X4 enemyCenterMat = XMMatrixTranslation(
            enemyBBox->m_fLengthX + enemyBBox->m_vMin.x,
            enemyBBox->m_fLengthY + enemyBBox->m_vMin.y,
            enemyBBox->m_fLengthZ + enemyBBox->m_vMin.z
        );
        enemyCenterMat = enemyCenterMat * enemyBBox->m_mWorld;
        VECTOR3 enemyCenter = GetPositionVector(enemyCenterMat);

        // 押し戻しベクトル（XZ平面）
        VECTOR3 pushDir = enemyCenter - stageCenter;
        pushDir.y = 0.0f;
        float dist = magnitude(pushDir);

        if (dist > 0.001f)
        {
            pushDir = normalize(pushDir);
            
            // 押し戻し距離（とりあえず固定値だが、エネミー同士よりは強く押し戻す）
            static constexpr float STAGE_PUSHBACK_DIST = 0.2f;
            pEnemy->AddPosition(pushDir * STAGE_PUSHBACK_DIST);
        }
    }
}
