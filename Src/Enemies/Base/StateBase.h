#pragma once
#include <queue>


class CEnemyBase;
class CComponentBase;


class CBaseState
{
public:
    
    CBaseState(CEnemyBase* e);
    virtual ~CBaseState() = default;

    enum class State : uint8_t
    {
        IDLE = 0,
        WALK,
        SUCTION,
        UNIQUEACTION,
        DESTROY,
        SEARCH,
        FIND_PLAYER,
        ESCAPE,
        HERDED, // 2. 隱伜ｰ弱＆繧後※縺・ｋ・育ｾ､繧後ｋ繝ｻ迥ｬ繧ФFO縺九ｉ騾・￡繧具ｼ・
        PANIC, // 3. 繝代ル繝・け・育堪縺後＞縺ｪ縺上↑縺｣縺ｦ證ｴ襍ｰ・・
        COLLECTING,
        DRIVING,
        RESCUE,
    };

    virtual void Enter(State type);
    virtual void Update();
    virtual void Exit();

    ///
    ///谺｡縺ｮstate縺鯉ｼ斐▽驟榊・縺ｫ蜈･縺｣縺ｦ縺ｪ縺九▲縺溘ｉ谺｡縺ｮ驟榊・繧偵Λ繝ｳ繝繝縺ｧ繧ｻ繝・ヨ
    ///
    void SetNextState();

protected:
    ///
    ///驟榊・縺九ｉ陦悟虚繧貞叙繧雁・縺励√◎繧後ｒ繧ｻ繝・ヨ
    ///
    CBaseState::State NextStatePop();

    static float ClampRotateY(const float& angle);
    CEnemyBase* m_pEnemy;
    State m_kType;
    CComponentBase* m_pComponent;

private:
    static constexpr int NEXT_STATE_MAX_SIZE = 3;
    std::queue<CBaseState::State> actionQueue;
};
