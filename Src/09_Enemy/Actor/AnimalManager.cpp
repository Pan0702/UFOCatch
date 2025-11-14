#include "AnimalManager.h"

#include "ACube.h"
#include "../../06_GameLib/BBox.h""
#include "../../08_Player/Player.h"
#include <thread>

#include "Human.h"

namespace
{
    constexpr float HALF_ROTATION_DEG = 180.0f;
    constexpr float FULL_ROTATION_DEG = 360.0f;
}
CAnimalManager::CAnimalManager()
{
}

CAnimalManager::~CAnimalManager()
{
}

void CAnimalManager::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_0))
    {
        new CACube(VECTOR3(0,0,0),VECTOR2(5,5));
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_9))
    {
        new CHuman();
    }
    
}


void CAnimalManager::Draw()
{
}

void CAnimalManager::HitCheck()
{
    SphereCollider coll;
    coll.center = VECTOR3(0, 0, 0);
    coll.radius = 1;
    MeshCollider::CollInfo info;
}


VECTOR3 CAnimalManager::GetObjectSize(MeshCollider* meshColl) const
{
    return meshColl->bBox.max;
}
void CAnimalManager::SetRotationY(const float& angle)
{
    float degAngle = angle * RadToDeg;

    while (degAngle > HALF_ROTATION_DEG)
    {
        degAngle -= FULL_ROTATION_DEG;
    }
    while (degAngle < -HALF_ROTATION_DEG)
    {
        degAngle += FULL_ROTATION_DEG;
    }

    transform.rotation.y = degAngle * DegToRad;
}


