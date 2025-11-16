#include "GameInstance.h"

CGameInstance::CGameInstance() : m_score(0), m_exp(0), m_allExp(0)
{
    ObjectManager::DontDestroy(this);		// DataCarrierは消されない
    ObjectManager::SetVisible(this, false);// DataCarrierは表示しない
    PushArry("Dog");
    m_score = 0;
}

CGameInstance::~CGameInstance() = default;

void CGameInstance::Update()
{
    Object3D::Update();
}

void CGameInstance::PushArry(const std::string& animalName)
{
    m_animalData.insert({animalName,0});
}

int CGameInstance::PopArry(const std::string& animalName)
{
    return m_animalData[animalName];
}

void CGameInstance::AddElement(const std::string& animalName)
{
    m_animalData[animalName]++;
}


