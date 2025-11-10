#pragma once
#include <unordered_map>
#include <vector>

#include "Stage.h"
#include "05_CommonFile/Object3D.h"

class CGameInstance : public Object3D
{
public:
    CGameInstance();
    ~CGameInstance();
    void PushArry(const std::string& animalName);
    int PopArry(const std::string& animalName);
    void AddElement(const std::string& animalName);

private:
    void Update() override;


private:
    int m_score;
    int m_exp;
    int m_allExp;

    std::unordered_map<std::string, int> m_animalData;
};
