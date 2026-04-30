#pragma once
#include <unordered_map>

#include "StateBase.h"
#include "../../Common/Object3D.h"
#include "../../Utils//BBox.h"
#include "../Component/ComponentBase.h"

class CEnemyManager;
class CStageObject;

class CEnemyBase : public Object3D
{
public:
    CEnemyBase();
    ~CEnemyBase();
    virtual void ChangeState(CBaseState::State type);
    void Update() override;
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    // 周辺のエネミーを取得
    std::vector<CEnemyBase*> GetNearbyEnemies() const;
    void SetRotateY(float y) { transform.rotation.y = y; }

    // ステージなどからの強制的な位置更新用
    void AddPosition(const VECTOR3& addPos) { transform.position += addPos; }

    virtual VECTOR3 SuctionSpeed() const;
    void IsSuctionCheck();

    // 壁スライディング：DesiredMoveから壁方向の成分を除いた移動ベクトルを返す
    VECTOR3 CalcSlideMove(const VECTOR3& desiredMove) const;

    const VECTOR2& GetAreaSize() const { return m_areaSize; }
    CBBox* GetBBox() const { return m_pBBox.get(); }
    CComponentBase* GetComponent(CBaseState::State type) const;

    bool IsHuman() const { return m_isHuman; }

    CBaseState::State GetCurrentState() const { return m_pState->GetCurrentState(); }

protected:
    // 物理演算
    void ApplyGravity(); // 重力を適用し、地面との衝突判定を行う
    virtual bool ShouldApplyGravity() const { return true; } // 重力を適用すべきかを判定

    // バウンディングボックス管理
    void UpdateBBox() const; // バウンディングボックスのワールド行列を更新
    std::unique_ptr<CBBox> CreateBBox(); // メッシュからバウンディングボックスを生成
    std::unique_ptr<CBBox> CreateBBox(float shrink);

    // OBB衝突判定と押し戻し処理
    void ResolveOBBCollisions(); // 周辺エネミーとのOBB衝突を検出し、押し戻し処理を実行
    virtual void CalcApplyPushback(CEnemyBase* other); // 衝突相手との押し戻しベクトルを計算して適用

    // ステージオブジェクトとの衝突判定と押し戻し処理
    void ResolveStageCollisions(); // ステージオブジェクトとのOBB衝突を検出し、押し戻し処理を実行

    std::unordered_map<CBaseState::State, std::unique_ptr<CComponentBase>> m_components;
    CComponentBase* m_pComponent = nullptr;
    std::unique_ptr<CBaseState> m_pState = nullptr;
    std::unique_ptr<CBBox> m_pBBox = nullptr;
    CPlayer* m_pPlayer = nullptr;
    CEnemyManager* m_pEnemyManager = nullptr;

    float m_velocityY;
    VECTOR2 m_areaSize;
    bool m_isHuman = false;
};
