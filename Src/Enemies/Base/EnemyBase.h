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
    virtual void SetState(CBaseState::State type);
    void Update() override;
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    // 周辺のエネミーを取得
    std::vector<CEnemyBase*> GetNearbyEnemies() const;
    

    CBBox* GetBBox() const { return m_pBBox; }
    void SetRotateY(float y)  { transform.rotation.y = y; }
    const VECTOR2& GetAreaSize() const { return m_areaSize; }
    // ステージなどからの強制的な位置更新用
    void AddPosition(const VECTOR3& addPos) { transform.position += addPos; }
    
    CComponentBase* GetComponent(CBaseState::State type) const;
    virtual VECTOR3 SuctionSpeed() const;
    void IsSuctionCheck();
    // 壁スライディング：desiredMoveから壁方向の成分を除いた移動ベクトルを返す
    VECTOR3 CalcSlideMove(const VECTOR3& desiredMove) const;
protected:
    // 物理演算
    void ApplyGravity();  // 重力を適用し、地面との衝突判定を行う
    virtual bool ShouldApplyGravity() const { return true; }  // 重力を適用すべきかを判定

    // バウンディングボックス管理
    void UpdateBBox() const;  // バウンディングボックスのワールド行列を更新
    CBBox* CreateBBox();  // メッシュからバウンディングボックスを生成

    // OBB衝突判定と押し戻し処理
    void ResolveOBBCollisions();  // 周辺エネミーとのOBB衝突を検出し、押し戻し処理を実行
    virtual void CalcApplyPushback(CEnemyBase* other);  // 衝突相手との押し戻しベクトルを計算して適用//

    // ステージオブジェクトとの衝突判定と押し戻し処理
    void ResolveStageCollisions();  // ステージオブジェクトとのOBB衝突を検出し、押し戻し処理を実行//

    std::unordered_map<CBaseState::State, CComponentBase*> m_components;
    CComponentBase* m_pComponent = nullptr;
    CBaseState* m_pState = nullptr;
    CBBox* m_pBBox = nullptr;
    CGround* m_pGround = nullptr;
    float m_velocityY;
    VECTOR2 m_areaSize;
};
