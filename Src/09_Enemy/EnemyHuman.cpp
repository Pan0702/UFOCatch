#include "EnemyHuman.h"

CEnemyHuman::CEnemyHuman()
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Ghosty/Ghost.mesh");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(0, "data/Ghosty/PacMan.anmx", true);
    m_pAnimator->Play(0);
    
    
}

CEnemyHuman::~CEnemyHuman()
{
}

void CEnemyHuman::Update()
{
    
}
