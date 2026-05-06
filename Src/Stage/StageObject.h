#pragma once
#include "../Common/Object3D.h"
#include "../Utils/BBox.h"
#include "StageCollision.h"

/// <summary>
/// ステージオブジェクト（静的な障害物、壁など）
/// OBB（BBox）による衝突判定を提供する
/// </summary>
class CStageObject : public Object3D
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="name">メッシュファイルのパス</param>
    /// <param name="pos">オブジェクトの位置</param>
    /// <param name="scale">オブジェクトのサイズ</param>
    /// <param name="soc">OBBを使用するか,地面との当たり判定を使うか（デフォルト: true）</param>
    CStageObject(const char* name, const VECTOR3& pos = VECTOR3(0, 0, 0), float scale = 1.0f,
                 StageColl soc = StageColl());
    CStageObject(const char* name, const Transform& t, StageColl soc = StageColl());

    void MakeOBB();

    void Update() override;
    void Draw() override;

    /// <summary>
    /// OBBとの衝突判定を行う
    /// 押し戻し法線はXZ平面のみ（Y成分=0）で返す
    /// </summary>
    /// <param name="other">相手のOBB</param>
    /// <param name="vHit">衝突位置（Out）</param>
    /// <param name="vNormal">衝突法線（Out）※XZ平面のみ</param>
    /// <returns>衝突していたらtrue</returns>
    bool HitOBB(CBBox* other, VECTOR3* vHit = nullptr, VECTOR3* vNormal = nullptr) const;

    /// <summary>
    /// OBBを取得
    /// </summary>
    /// <returns>OBBのポインタ</returns>
    CBBox* GetOBB() const { return m_pOBB.get(); }

    // 四分木登録用：XZ平面でのAABB中心と大きさを返す
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    /// <summary>
    /// エネミーとの衝突を解決する
    /// </summary>
    /// <param name="pEnemy">判定対象のエネミー</param>
    void ResolveEnemyCollision(class CEnemyBase* pEnemy);

    bool HitGround(const VECTOR3& rayStart, const VECTOR3& rayEnd,
                   MeshCollider::CollInfo* outInfo) const;
    bool MayHitGround(float fromY, float toY) const;

    void SetIsHitFlag(bool flag);
    bool GetIsHitFlag() const;

private:
    std::unique_ptr<CBBox> m_pOBB; // OBB衝突判定
    bool m_useOBB; // OBBを使用するか
    bool m_isHitGround; //床判定に使うか
};
