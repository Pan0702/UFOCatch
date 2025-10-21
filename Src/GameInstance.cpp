#include "GameInstance.h"

GameInstance::GameInstance()
{
}

GameInstance::~GameInstance() = default;

void GameInstance::Update()
{
    Object3D::Update();
}

void GameInstance::InitArry(const std::string& animalName, const int& arryElement)
{
    m_animalData.resize(arryElement);
    for (int i = 0; i < arryElement; i++)
    {
        m_animalData[i].m_sAnimalName = animalName;
        m_animalData[i].m_SuckUpAnimalNum = 0;
    }
}

void GameInstance::AddSuckUpAnimalNum(const std::string& animalName)
{
    for (auto CurrentName : m_animalData)
    {
       if (CurrentName.m_sAnimalName == animalName)
       {
           CurrentName.m_SuckUpAnimalNum++;
       }
    }
}
