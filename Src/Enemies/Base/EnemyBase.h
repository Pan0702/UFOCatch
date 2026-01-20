#pragma once
#include <unordered_map>
#include <vector>

#include "StateBase.h"
#include "../../Common/Object3D.h"
#include "../../Utils//BBox.h"
#include "../../Stage/Ground.h"

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
    
    // ステージなどからの強制的な位置更新用
    void AddPosition(const VECTOR3& addPos) { transform.position += addPos; }

protected:
    // 物理演算
    void ApplyGravity();  // 重力を適用し、地面との衝突判定を行う
    virtual bool ShouldApplyGravity() const { return true; }  // 重力を適用すべきかを判定

    // バウンディングボックス管理
    void UpdateBBox();  // バウンディングボックスのワールド行列を更新
    CBBox* CreateBBox();  // メッシュからバウンディングボックスを生成

    // OBB衝突判定と押し戻し処理
    void ResolveOBBCollisions();  // 周辺エネミーとのOBB衝突を検出し、押し戻し処理を実行
    virtual void CalcApplyPushback(CEnemyBase* other);  // 衝突相手との押し戻しベクトルを計算して適用

    // ステージオブジェクトとの衝突判定と押し戻し処理
    void ResolveStageCollisions();  // ステージオブジェクトとのOBB衝突を検出し、押し戻し処理を実行
    

    CBaseState* m_pCurrentState;
    std::unordered_map<CBaseState::Type, CBaseState*> m_cubeStates;
    CBBox* m_pBBox;
    CGround* m_pGround;
    
    float m_velocityY;
};
