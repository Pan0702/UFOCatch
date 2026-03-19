#pragma once
#include "../Common/Object3D.h"
#include "../Utils/BBox.h"

/// <summary>
/// 繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝茨ｼ磯撕逧・↑髫懷ｮｳ迚ｩ縲∝｣√↑縺ｩ・・
/// OBB・・BBox・峨↓繧医ｋ陦晉ｪ∝愛螳壹ｒ謠蝉ｾ・
/// </summary>
class CStageObject : public Object3D
{
public:
    /// <summary>
    /// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
    /// </summary>
    /// <param name="name">繝｡繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ</param>
    /// <param name="pos">繧ｪ繝悶ず繧ｧ繧ｯ繝医・菴咲ｽｮ</param>
    /// <param name="scale">繧ｪ繝悶ず繧ｧ繧ｯ繝医・繧ｵ繧､繧ｺ</param>
    /// <param name="useOBB">OBB繧剃ｽｿ逕ｨ縺吶ｋ縺具ｼ医ョ繝輔か繝ｫ繝・ true・・/param>
    CStageObject(const char* name, const VECTOR3& pos = VECTOR3(0,0,0), float scale = 1.0f, bool useOBB = true);
    virtual ~CStageObject();

    void Update() override;
    void Draw() override;

    /// <summary>
    /// OBB縺ｨ縺ｮ陦晉ｪ∝愛螳壹ｒ陦後≧
    /// 謚ｼ縺玲綾縺玲ｳ慕ｷ壹・XZ蟷ｳ髱｢縺ｮ縺ｿ・・謌仙・=0・峨〒霑斐☆
    /// </summary>
    /// <param name="other">逶ｸ謇九・OBB</param>
    /// <param name="vHit">陦晉ｪ∽ｽ咲ｽｮ・・ut・・/param>
    /// <param name="vNormal">陦晉ｪ∵ｳ慕ｷ夲ｼ・ut・俄ｻXZ蟷ｳ髱｢縺ｮ縺ｿ</param>
    /// <returns>陦晉ｪ√＠縺ｦ縺・◆繧液rue</returns>
    bool HitOBB(CBBox* other, VECTOR3* vHit = nullptr, VECTOR3* vNormal = nullptr);

    /// <summary>
    /// OBB繧貞叙蠕・
    /// </summary>
    /// <returns>OBB縺ｮ繝昴う繝ｳ繧ｿ</returns>
    CBBox* GetOBB() { return m_pOBB; }

    // 蝗帛・譛ｨ逋ｻ骭ｲ逕ｨ・唸Z蟷ｳ髱｢縺ｧ縺ｮAABB荳ｭ蠢・→螟ｧ縺阪＆繧定ｿ斐☆
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    /// <summary>
    /// 繧ｨ繝阪Α繝ｼ縺ｨ縺ｮ陦晉ｪ√ｒ隗｣豸医☆繧・
    /// </summary>
    /// <param name="pEnemy">蛻､螳壼ｯｾ雎｡縺ｮ繧ｨ繝阪Α繝ｼ</param>
    void ResolveEnemyCollision(class CEnemyBase* pEnemy);

protected:
    CBBox* m_pOBB;      // OBB陦晉ｪ∝愛螳・
    bool   m_bUseOBB;   // OBB繧剃ｽｿ逕ｨ縺吶ｋ縺・
};

