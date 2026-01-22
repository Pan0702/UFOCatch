#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class CWalk : public CBaseState
{
public:
    CWalk(CEnemyBase* e);
    void Enter() override;
    void Update() override;
    void Exit() override;
};
class CIdle : public CBaseState
{
public:
    CIdle(CEnemyBase* e);
    void Enter() override;
    void Update() override;
    void Exit() override;
};

class CSuction : public CBaseState
{
public:
    CSuction(CEnemyBase* e);
    void Enter() override;
    void Update() override;
    void Exit() override;
};

class CUniqueAction : public CBaseState
{
public:
    CUniqueAction(CEnemyBase* e);
    void Enter() override;
    void Update() override;
    void Exit() override;
};

