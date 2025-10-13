#pragma once
#include <chrono>

#include "../CommonFile/Object3D.h"
#include "../Player/Player.h"

class CAnimalManager:public Object3D
{
public:
    CAnimalManager();
    ~CAnimalManager();
protected:
    //各座標の最大値を返却
     VECTOR3 GetObjectSize(MeshCollider* meshColl) const;
    CPlayer* m_pPlayer;
private:
    void Update() override;
    void Draw() override;
    
    
};
