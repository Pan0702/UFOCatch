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
    /// CStageObject を初期化する
    /// @param name 名前
    /// @param pos 座標
    /// @param scale スケール
    /// @param soc soc に渡す値
    CStageObject(const char* name, const VECTOR3& pos = VECTOR3(0, 0, 0), float scale = 1.0f,
                 StageColl soc = StageColl());
    /// CStageObject を初期化する
    /// @param name 名前
    /// @param t t に渡す値
    /// @param soc soc に渡す値
    CStageObject(const char* name, const Transform& t, StageColl soc = StageColl());

    /// OBB を作成する
    void MakeOBB();

    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

    /// Hit OBB を返す
    /// @param other other に渡す値
    /// @param vHit vHit に渡す値
    /// @param vNormal vNormal に渡す値
    /// @return 成功または条件を満たす場合 true
    bool HitOBB(CBBox* other, VECTOR3* vHit = nullptr, VECTOR3* vNormal = nullptr) const;

    /// OBB を取得する
    /// @return 対象のポインタ
    CBBox* GetOBB() const { return m_pOBB.get(); }

    // 四分木登録用：XZ平面でのAABB中心と大きさを返す
    /// Bounds2 D を取得する
    /// @param outPos 座標
    /// @param outSize サイズ
    /// @return 成功または条件を満たす場合 true
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    /// Resolve Enemy Collision の処理を行う
    /// @param pEnemy pEnemy に渡す値
    void ResolveEnemyCollision(class CEnemyBase* pEnemy);

    /// Hit Ground を返す
    /// @param rayStart 判定に使用するレイ
    /// @param rayEnd 判定に使用するレイ
    /// @param outInfo outInfo に渡す値
    /// @return 成功または条件を満たす場合 true
    bool HitGround(const VECTOR3& rayStart, const VECTOR3& rayEnd,
                   MeshCollider::CollInfo* outInfo) const;
    /// May Hit Ground を返す
    /// @param fromY fromY に渡す値
    /// @param toY toY に渡す値
    /// @return 成功または条件を満たす場合 true
    bool MayHitGround(float fromY, float toY) const;

    /// Is Hit Flag を設定する
    /// @param flag flag に渡す値
    void SetIsHitFlag(bool flag);
    /// Is Hit Flag を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetIsHitFlag() const;

private:
    std::unique_ptr<CBBox> m_pOBB; // OBB衝突判定
    bool m_useOBB; // OBBを使用するか
    bool m_isHitGround; //床判定に使うか
};
