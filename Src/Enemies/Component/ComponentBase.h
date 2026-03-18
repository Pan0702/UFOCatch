#pragma once
#include "../../Player/Player.h"
#include "../System/EnemyManager.h"
#include "../Base/EnemyBase.h"

class CComponentBase
{
public:
    CComponentBase() = default;
    virtual ~CComponentBase()
    {
        // m_pPlayer 縺ｨ m_pOwner 縺ｯ蜿ら・逕ｨ繝昴う繝ｳ繧ｿ縺ｪ縺ｮ縺ｧ蜑企勁縺励↑縺・
        // 螳滄圀縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・蛻･縺ｮ蝣ｴ謇縺ｧ邂｡逅・＆繧後※縺・ｋ
    }
    
    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit()
    {
        m_isFinish = false;
    }
    
    bool IsFinish() const{return m_isFinish;}
    
protected:
    CPlayer* m_pPlayer;
    CEnemyBase* m_pOwner;
    bool m_isFinish;

};

