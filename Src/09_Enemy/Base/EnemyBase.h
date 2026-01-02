#pragma once
#include <unordered_map>

#include "StateBase.h"
#include "../../05_CommonFile/Object3D.h"
#include "../../06_GameLib/BBox.h"


class CGround;

class CEnemyBase : public Object3D
{
public:
    CEnemyBase();
    virtual void SetState(CBaseState::Type type);

    ~CEnemyBase();
    void Update() override;
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    // 周辺のエネミーを取得
    std::vector<CEnemyBase*> GetNearbyEnemies() const;
    
    CBBox* GetBBox() const { return m_pBBox; }

protected:
    void ApplyGravity();
    bool IsGrounded() const;
    virtual bool ShouldApplyGravity() const { return true; }
    void UpdateBBox();
    CBBox* CreateBBox();
    

    CBaseState* m_pCurrentState;
    std::unordered_map<CBaseState::Type, CBaseState*> m_cubeStates;
    CBBox* m_pBBox;
    CGround* m_pGround;
    
    float m_velocityY;
};
