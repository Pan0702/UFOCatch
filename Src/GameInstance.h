#pragma once
#include <vector>

#include "Stage.h"
#include "05_CommonFile/Object3D.h"

class GameInstance : public Object3D
{
public:
    GameInstance();
    ~GameInstance();
private:
    void Update() override;
    void InitArry(const std::string& animalName,const int& arryElement);
    void AddSuckUpAnimalNum(const std::string& animalName);
    
    struct AnimalData
    {
        std::string m_sAnimalName;
        int m_SuckUpAnimalNum;
  };
    std::vector<AnimalData> m_animalData;
};
