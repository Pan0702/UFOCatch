#pragma once
#include <queue>

#include "EnemyBase.h"

class CBaseState
{
public:
    CBaseState(CEnemyBase* e);
    ~CBaseState();

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

    virtual void Enter(Type type);
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
    CBaseState::Type NextStatePop();

    float ClampRotateY(const float& angle);
    CEnemyBase* m_pEnemy;
    Type m_kType;
    CComponentBase* m_pComponent = nullptr;
    
    
private:

    static constexpr int NEXT_STATE_MAX_SIZE = 3;
    std::queue<CBaseState::Type> actionQueue;
    

};
