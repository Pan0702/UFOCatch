#pragma once
#include <chrono>
#include "../State/BaseState.h"
#include "../../05_CommonFile/Object3D.h"
#include "../../08_Player/Player.h"

class CAnimalManager : public Object3D
{
public:
    CAnimalManager();
    ~CAnimalManager();

    //各座標の最大値を返却
    VECTOR3 GetObjectSize(MeshCollider* meshColl) const;
    virtual void  SetRotationY(const float& angle);
public:
    class CPlayer* m_pPlayer;
    

private:
    void Update() override;
    void Draw() override;
    void HitCheck();


};

enum AnimationType 
{
    A_IDEL = 0,
    A_WALK,
    A_RUN,
    A_SEACH
};