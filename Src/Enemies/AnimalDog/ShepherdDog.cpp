#include "ShepherdDog.h"

void CAShepherdDog::Update()
{
    FlogInfo info = ObjectManager::FindGameObject<Flog>()->CalcFlogInfo(sheeps);
    if (info.maxDistance > 10.0f)Collecting(info);
}

void CAShepherdDog::ChangeStateHerded(CSheep* sheep)
{
}

void CAShepherdDog::Collecting(const FlogInfo& flogInfo)
{
}

void CAShepherdDog::Driving(const FlogInfo& flogInfo)
{
}

