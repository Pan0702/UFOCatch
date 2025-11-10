#pragma once
#include <queue>

#include "../../08_Player/Player.h"
class CBaseState
{
public:
    enum class Type
    {
        Idle,
        Walk,
        Suction,
        Destroy,
        LookAround,
        Escape
    };

    virtual ~CBaseState(){}
    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit(){}

    ///
    ///次のstateが４つ配列に入ってなかったら次の配列をランダムでセット
    ///
    void SetNextState();
protected:
    
    CBaseState(){}
    ///
    ///配列から行動を取り出し、それをセット
    ///
    CBaseState::Type NextStatePop();
private:
    static constexpr int NEXT_STATE_MAX_SIZE = 3;
    std::queue<CBaseState::Type> actionQueue;
    
};
