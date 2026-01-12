#pragma once
#include "../05_CommonFile/Object3D.h"
#include "../06_GameLib/BBox.h"

/// <summary>
/// ステージオブジェクト（静的な障害物、壁など）
/// OBB（CBBox）による衝突判定を提供
/// </summary>
class CStageObject : public Object3D
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="meshPath">メッシュファイルのパス</param>
    /// <param name="pos">オブジェクトの位置</param>
    /// <param name="useOBB">OBBを使用するか（デフォルト: true）</param>
    CStageObject(const char* meshPath, const VECTOR3& pos = VECTOR3(0,0,0), bool useOBB = true);


    virtual ~CStageObject();

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
    bool HitOBB(CBBox* other, VECTOR3* vHit = nullptr, VECTOR3* vNormal = nullptr);

    /// <summary>
    /// OBBを取得
    /// </summary>
    /// <returns>OBBのポインタ</returns>
    CBBox* GetOBB() { return m_pOBB; }

    /// <summary>
    /// エネミーとの衝突を解消する
    /// </summary>
    /// <param name="pEnemy">判定対象のエネミー</param>
    void ResolveEnemyCollision(class CEnemyBase* pEnemy);

protected:
    CBBox* m_pOBB;      // OBB衝突判定
    bool   m_bUseOBB;   // OBBを使用するか
};
