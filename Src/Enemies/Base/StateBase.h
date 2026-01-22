#pragma once
#include <queue>

#include "EnemyBase.h"

class CBaseState
{
public:
    enum class Type
    {
        IDLE,
        WALK,
        SUCTION,
        UNIQUEACTION,
        DESTROY,
        SEARCH,
        FIND_PLAYER,
        ESCAPE
    };
    CBaseState(CEnemyBase* cube, Type type);
    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit(){}

    ///
    ///次のstateが４つ配列に入ってなかったら次の配列をランダムでセット
    ///
    void SetNextState();
protected:
    ///
    ///配列から行動を取り出し、それをセット
    ///
    CBaseState::Type NextStatePop();

    float ClampRotateY(const float& angle);
private:

    static constexpr int NEXT_STATE_MAX_SIZE = 3;
    std::queue<CBaseState::Type> actionQueue;
    
    CEnemyBase* m_pEnemy;
    const Type m_kType;
};
