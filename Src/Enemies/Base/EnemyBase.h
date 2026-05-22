#pragma once
#include <unordered_map>

#include "StateBase.h"
#include "../../Common/Object3D.h"
#include "../../Utils//BBox.h"
#include "../Component/ComponentBase.h"

/// <summary>敵AIで使う Enemy Manager の情報と処理をまとめる型</summary>
class CEnemyManager;
/// <summary>敵AIで使う Stage Object の情報と処理をまとめる型</summary>
class CStageObject;

/// <summary>敵キャラクター共通の移動、衝突、状態制御を持つ基底クラス</summary>
class CEnemyBase : public Object3D
{
public:
    /// CEnemyBase を初期化する
    CEnemyBase();
    /// CEnemyBase の終了処理を行う
    ~CEnemyBase();
    /// State を切り替える
    /// @param type type に渡す値
    virtual void ChangeState(CBaseState::State type);
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Bounds2 D を取得する
    /// @param outPos 座標
    /// @param outSize サイズ
    /// @return 成功または条件を満たす場合 true
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    // 周辺のエネミーを取得
    /// Nearby Enemies を取得する
    /// @return 取得した要素一覧
    std::vector<CEnemyBase*> GetNearbyEnemies() const;
    /// Rotate Y を設定する
    /// @param y y に渡す値
    void SetRotateY(float y) { transform.rotation.y = y; }

    // ステージなどからの強制的な位置更新用
    /// Position を追加する
    /// @param addPos 座標
    void AddPosition(const VECTOR3& addPos) { transform.position += addPos; }

    /// Suction Speed を返す
    /// @return 3次元ベクトル
    virtual VECTOR3 SuctionSpeed() const;
    /// Suction Check を判定する
    void IsSuctionCheck();

    // 壁スライディング：DesiredMoveから壁方向の成分を除いた移動ベクトルを返す
    /// Slide Move を計算する
    /// @param desiredMove desiredMove に渡す値
    /// @return 3次元ベクトル
    VECTOR3 CalcSlideMove(const VECTOR3& desiredMove) const;

    /// Area Size を取得する
    /// @return 2次元ベクトル
    const VECTOR2& GetAreaSize() const { return m_areaSize; }
    /// BBox を取得する
    /// @return 対象のポインタ
    CBBox* GetBBox() const { return m_pBBox.get(); }
    /// Component を取得する
    /// @param type type に渡す値
    /// @return 対象のポインタ
    CComponentBase* GetComponent(CBaseState::State type) const;

    /// Human を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsHuman() const { return m_isHuman; }

    /// Current State を取得する
    /// @return 処理結果
    CBaseState::State GetCurrentState() const { return m_pState->GetCurrentState(); }

protected:
    // 物理演算
    /// Apply Gravity の処理を行う
    void ApplyGravity(); // 重力を適用し、地面との衝突判定を行う
    /// Should Apply Gravity を返す
    /// @return 成功または条件を満たす場合 true
    virtual bool ShouldApplyGravity() const { return true; } // 重力を適用すべきかを判定

    // バウンディングボックス管理
    /// BBox を毎フレームの状態を更新する
    void UpdateBBox() const; // バウンディングボックスのワールド行列を更新
    /// BBox を作成する
    /// @return 処理結果
    std::unique_ptr<CBBox> CreateBBox(); // メッシュからバウンディングボックスを生成
    /// BBox を作成する
    /// @param shrink shrink に渡す値
    /// @return 処理結果
    std::unique_ptr<CBBox> CreateBBox(float shrink);

    // OBB衝突判定と押し戻し処理
    /// Resolve OBBCollisions の処理を行う
    void ResolveOBBCollisions(); // 周辺エネミーとのOBB衝突を検出し、押し戻し処理を実行
    /// Apply Pushback を計算する
    /// @param other other に渡す値
    virtual void CalcApplyPushback(CEnemyBase* other); // 衝突相手との押し戻しベクトルを計算して適用

    // ステージオブジェクトとの衝突判定と押し戻し処理
    /// Resolve Stage Collisions の処理を行う
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
