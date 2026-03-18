#pragma once
#include <unordered_map>

#include "StateBase.h"
#include "../../Common/Object3D.h"
#include "../../Utils//BBox.h"
#include "../../Stage/Ground.h"
#include "../Component/ComponentBase.h"

class CEnemyBase : public Object3D
{
public:
    CEnemyBase();
    ~CEnemyBase();
    virtual void ChangeState(CBaseState::State type) ;
    void Update() override;
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    // 蜻ｨ霎ｺ縺ｮ繧ｨ繝阪Α繝ｼ繧貞叙蠕・
    std::vector<CEnemyBase*> GetNearbyEnemies() const;
    void SetRotateY(float y)  { transform.rotation.y = y; }

    // 繧ｹ繝・・繧ｸ縺ｪ縺ｩ縺九ｉ縺ｮ蠑ｷ蛻ｶ逧・↑菴咲ｽｮ譖ｴ譁ｰ逕ｨ
    void AddPosition(const VECTOR3& addPos) { transform.position += addPos; }
    
    virtual VECTOR3 SuctionSpeed() const;
    void IsSuctionCheck();
    
    // 螢√せ繝ｩ繧､繝・ぅ繝ｳ繧ｰ・單esiredMove縺九ｉ螢∵婿蜷代・謌仙・繧帝勁縺・◆遘ｻ蜍輔・繧ｯ繝医Ν繧定ｿ斐☆
    VECTOR3 CalcSlideMove(const VECTOR3& desiredMove) const;
    
    const VECTOR2& GetAreaSize() const { return m_areaSize; }
    CBBox* GetBBox() const { return m_pBBox.get(); }
    CComponentBase* GetComponent(CBaseState::State type) const;
    
    bool IsHuman() const { return m_isHuman; }
protected:
    // 迚ｩ逅・ｼ皮ｮ・
    void ApplyGravity();  // 驥榊鴨繧帝←逕ｨ縺励∝慍髱｢縺ｨ縺ｮ陦晉ｪ∝愛螳壹ｒ陦後≧
    virtual bool ShouldApplyGravity() const { return true; }  // 驥榊鴨繧帝←逕ｨ縺吶∋縺阪°繧貞愛螳・

    // 繝舌え繝ｳ繝・ぅ繝ｳ繧ｰ繝懊ャ繧ｯ繧ｹ邂｡逅・
    void UpdateBBox() const;  // 繝舌え繝ｳ繝・ぅ繝ｳ繧ｰ繝懊ャ繧ｯ繧ｹ縺ｮ繝ｯ繝ｼ繝ｫ繝芽｡悟・繧呈峩譁ｰ
    std::unique_ptr<CBBox> CreateBBox();  // 繝｡繝・す繝･縺九ｉ繝舌え繝ｳ繝・ぅ繝ｳ繧ｰ繝懊ャ繧ｯ繧ｹ繧堤函謌・

    // OBB陦晉ｪ∝愛螳壹→謚ｼ縺玲綾縺怜・逅・
    void ResolveOBBCollisions();  // 蜻ｨ霎ｺ繧ｨ繝阪Α繝ｼ縺ｨ縺ｮOBB陦晉ｪ√ｒ讀懷・縺励∵款縺玲綾縺怜・逅・ｒ螳溯｡・
    virtual void CalcApplyPushback(CEnemyBase* other);  // 陦晉ｪ∫嶌謇九→縺ｮ謚ｼ縺玲綾縺励・繧ｯ繝医Ν繧定ｨ育ｮ励＠縺ｦ驕ｩ逕ｨ//

    // 繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ陦晉ｪ∝愛螳壹→謚ｼ縺玲綾縺怜・逅・
    void ResolveStageCollisions();  // 繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮOBB陦晉ｪ√ｒ讀懷・縺励∵款縺玲綾縺怜・逅・ｒ螳溯｡・/

    std::unordered_map<CBaseState::State, std::unique_ptr<CComponentBase>> m_components;
    CComponentBase* m_pComponent = nullptr;
    std::unique_ptr<CBaseState> m_pState = nullptr;
    std::unique_ptr<CBBox> m_pBBox = nullptr;
    CGround* m_pGround = nullptr;
    CPlayer* m_pPlayer = nullptr;
    CEnemyManager* m_pEnemyManager = nullptr;
    
    float m_velocityY;
    VECTOR2 m_areaSize;
    bool m_isHuman = false;
};

