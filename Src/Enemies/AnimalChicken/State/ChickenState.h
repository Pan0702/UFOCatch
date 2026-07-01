#include "../../Base/StateBase.h"
#include "../../../Common/Object3D.h"
#include "../../../Player/Player.h"
/// <summary>敵AIで使う Animal Chicken の情報と処理をまとめる型</summary>
class CAnimalChicken;

/// <summary>敵AIで使う Chicken Base の情報と処理をまとめる型</summary>
class CChickenBase : public CBaseState
{
public:
    /// CChickenBase を初期化する
    /// @param chicken chicken に渡す値
    /// @param type type に渡す値
    CChickenBase(CAnimalChicken* chicken, State type);
    /// CChickenBase の終了処理を行う
    ~CChickenBase();
    CAnimalChicken* m_pOwner;
    const State m_kType;

protected:
    /// Next State の処理を行う
    void NextState();
};

/// <summary>敵AIで使う Chicken Idle State の情報と処理をまとめる型</summary>
class CChickenIdleState : public CChickenBase
{
public:
    /// CChickenIdleState を初期化する
    /// @param chicken chicken に渡す値
    CChickenIdleState(CAnimalChicken* chicken);
    /// Enter の処理を行う
    /// @param type type に渡す値
    void Enter(State type) override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Idle の処理を行う
    void Idle();
    /// Idle Anim の処理を行う
    void IdleAnim();
    /// Animation Finish を返す
    /// @return 成功または条件を満たす場合 true
    bool AnimationFinish() const;

private:
    float timerCount;
    int stateIdle;
};

/// <summary>敵AIで使う Chicken Walk State の情報と処理をまとめる型</summary>
class CChickenWalkState : public CChickenBase
{
public:
    /// CChickenWalkState を初期化する
    /// @param chicken chicken に渡す値
    CChickenWalkState(CAnimalChicken* chicken);
    /// Enter の処理を行う
    /// @param type type に渡す値
    void Enter(State type) override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Boundary Check を返す
    /// @param areaSize サイズ
    /// @return 成功または条件を満たす場合 true
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    VECTOR3 BASE_POS;
    float m_turnAmount;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    VECTOR3 m_position;
    bool m_rotation;
    float m_currentRotation;
    float m_targetRotation;
};

/// <summary>敵AIで使う Chicken Suction の情報と処理をまとめる型</summary>
class CChickenSuction : public CChickenBase
{
public:
    /// CChickenSuction を初期化する
    /// @param chicken chicken に渡す値
    CChickenSuction(CAnimalChicken* chicken);
    /// Enter の処理を行う
    /// @param type type に渡す値
    void Enter(State type) override;
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    CPlayer* m_pPlayer;
    VECTOR3 m_distanceFromObjectToUFO;
};

/// <summary>敵AIで使う Chicken Destroy の情報と処理をまとめる型</summary>
class CChickenDestroy : public CChickenBase
{
public:
    /// CChickenDestroy を初期化する
    /// @param chicken chicken に渡す値
    CChickenDestroy(CAnimalChicken* chicken);
    /// Enter の処理を行う
    /// @param type type に渡す値
    void Enter(State type) override;
};
