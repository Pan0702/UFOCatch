#pragma once
#include <queue>


class CEnemyBase;
class CComponentBase;

namespace
{
}

class CBaseState
{
public:
    CBaseState(CEnemyBase* e);
    ~CBaseState();

    enum State
    {
        IDLE = 0,
        WALK,
        SUCTION,
        UNIQUEACTION,
        DESTROY,
        SEARCH,
        FIND_PLAYER,
        ESCAPE,
        HERDED, // 2. 誘導されている（群れる・犬やUFOから逃げる）
        PANIC, // 3. パニック（犬がいなくなって暴走）
    };

    virtual void Enter(State type);
    virtual void Update();
    virtual void Exit();

    ///
    ///次のstateが４つ配列に入ってなかったら次の配列をランダムでセット
    ///
    void SetNextState();

protected:
    ///
    ///配列から行動を取り出し、それをセット
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
