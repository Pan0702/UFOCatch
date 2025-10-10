#pragma once
#include "Object3D.h"

struct AnimalInfo;

class Stage : public Object3D
{
public:
    Stage();
    ~Stage();

public:
    int depthStageSize;
    int widthStageSize;
private:
    void Update() override;
    void Draw() override;

private:
    VECTOR2 m_stageSize;
};

struct StageInfo
{
    int stageNum;
    std::vector<AnimalInfo> s_animalInfo;
};

struct AnimalInfo
{
    int animalNum;
    VECTOR3 animalPos;
};
