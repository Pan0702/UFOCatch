#pragma once
#include "../Framework/GameObject.h"

// 繧ｹ繝・・繧ｸ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ逕滓・縺吶ｋ繝輔ぃ繧ｯ繝医Μ繝ｼ繧ｯ繝ｩ繧ｹ //
class CStageFactor : public GameObject
{
public:
    CStageFactor();
    static void SpawnObjects(float sizeX, float sizeZ, int num);;
};

