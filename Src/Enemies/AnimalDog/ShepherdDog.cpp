#include "ShepherdDog.h"

CAShepherdDog::CAShepherdDog()
{
    
    m_components[CBaseState::State::] = new CCollecting(this);
}

void CAShepherdDog::Update()
{
    if (!m_isHerding)return;
    FlogInfo info = ObjectManager::FindGameObject<Flog>()->CalcFlogInfo(m_sheeps);
    if (info.maxDistance > 10.0f)
    {
        SetState(CBaseState::State::COLLECTING);
    } else {
        SetState(CBaseState::State::DRIVING);
    }
}

void CAShepherdDog::ChangeStateHerded(const CSheep* sheep) const
{
    for (auto s : m_sheeps)
    {
        if (s == sheep)continue;
        s->SetState(CBaseState::State::HERDED);
    }
}

void CAShepherdDog::Collecting(const FlogInfo& flogInfo)
{
    VECTOR3 sheepPos = flogInfo.furthestSheep->GetTransform().position;
    VECTOR3 toCentroid = flogInfo.centroid - sheepPos;
    normalize(toCentroid);
    float behindDistance = 10.0f;
    VECTOR3 targetPos = sheepPos - toCentroid * behindDistance;

    MoveTo(targetPos);
}

void CAShepherdDog::Driving(const FlogInfo& flogInfo)
{
    const VECTOR3 plPos = ObjectManager::FindGameObject<CPlayer>()->GetTransform().position;
    const VECTOR3 centroid = flogInfo.centroid;
    
    VECTOR3 escapeDir  = centroid - plPos;
    normalize(escapeDir);  
    
    float interceptDistance = 15.0f;  // 群れからどれくらい離れるか
    VECTOR3 targetPos = centroid - escapeDir * interceptDistance;
    
    MoveTo(targetPos);
}

void CAShepherdDog::StartHerding()
{
    m_isHerding = true;
}
