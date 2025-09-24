#include "PCamera.h"

namespace
{
    const VECTOR3 initUpDir = VECTOR3(0, 1, 0);
    const VECTOR3 initCamPos =  VECTOR3(2, 4, -10) ;
    const VECTOR3 initCamLook =  VECTOR3(0, 2, 0) ; 
}

CPlayerCamera::CPlayerCamera()
{

}

CPlayerCamera::~CPlayerCamera()
{
}

void CPlayerCamera::Update()
{
    GameDevice()->m_mView = XMMatrixLookAtLH(
    m_camPos, m_camLook, initCamLook);
}

void CPlayerCamera::PosSet(const VECTOR3& pos)
{
    m_camPos = pos + initCamPos;
    m_camLook = pos + initCamLook;
}

