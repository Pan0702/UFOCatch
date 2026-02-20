#include "StageObject.h"
#include "../Enemies/Base/EnemyBase.h"
#include "../Framework/ResourceManager.h"

////////////////////
// コンストラクタ
// @param meshPath メッシュファイルのパス
// @param pos オブジェクトの位置
// @param scale オブジェクトのサイズ
// @param useOBB OBBを使用するか //
////////////////////
CStageObject::CStageObject(const char* meshPath, const VECTOR3& pos,float scale, bool useOBB)
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
    transform.position = pos;
    transform.scale = VECTOR3(1.0f, 1.0f, 1.0f) * scale;
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
    
    // // デバッグ用: OBBを描画（必要に応じてコメントアウト）
    // if (m_pOBB)
    // {
    //     m_pOBB->Render();
    // }
}

////////////////////
// OBBとの衝突判定を行う
// @param other 相手のOBB
// @param vHit 衝突位置（Out）
// @param vNormal 衝突法線（Out）※XZ平面のみ（Y成分は0）
// @return 衝突していたらtrue //
////////////////////
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

////////////////////
// エネミーとの衝突を解消する
// @param pEnemy 判定対象のエネミー //
////////////////////
void CStageObject::ResolveEnemyCollision(CEnemyBase* pEnemy)
{
    if (!m_pOBB || !pEnemy || !pEnemy->GetBBox()) return;

    VECTOR3 hitPos, hitNormal;
    if (HitOBB(pEnemy->GetBBox(), &hitPos, &hitNormal))
    {
        // OBBCollisionDetection は法線を (0,1,0) 固定で返すため hitNormal は使用不可
        // ステージ中心 → エネミー中心 の方向で押し戻す
        MATRIX4X4 stageCenterMat = XMMatrixTranslation(
            m_pOBB->m_fLengthX + m_pOBB->m_vMin.x,
            m_pOBB->m_fLengthY + m_pOBB->m_vMin.y,
            m_pOBB->m_fLengthZ + m_pOBB->m_vMin.z
        );
        stageCenterMat = stageCenterMat * m_pOBB->m_mWorld;
        VECTOR3 stageCenter = GetPositionVector(stageCenterMat);

        CBBox* enemyBBox = pEnemy->GetBBox();
        MATRIX4X4 enemyCenterMat = XMMatrixTranslation(
            enemyBBox->m_fLengthX + enemyBBox->m_vMin.x,
            enemyBBox->m_fLengthY + enemyBBox->m_vMin.y,
            enemyBBox->m_fLengthZ + enemyBBox->m_vMin.z
        );
        enemyCenterMat = enemyCenterMat * enemyBBox->m_mWorld;
        VECTOR3 enemyCenter = GetPositionVector(enemyCenterMat);

        VECTOR3 pushDir = enemyCenter - stageCenter;
        pushDir.y = 0.0f;
        float dist = magnitude(pushDir);
        if (dist > 0.001f)
        {
            pushDir = normalize(pushDir);
            static constexpr float STAGE_PUSHBACK_DIST = 0.2f;
            pEnemy->AddPosition(pushDir * STAGE_PUSHBACK_DIST);
        }
    }
}
