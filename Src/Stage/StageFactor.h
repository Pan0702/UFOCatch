#pragma once
#include "../Framework/GameObject.h"

// ステージのオブジェクトを生成するファクトリークラス //
class CStageFactor : public GameObject
{
public:
    CStageFactor();
    static void SpawnObjects(float sizeX, float sizeZ, int num);;
};
