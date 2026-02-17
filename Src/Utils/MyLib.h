#pragma once
#include "MyMath.h"

namespace MyLib
{
    // 引き寄せるための移動量を計算する
    // 高さの差が大きいほど遅く、近いほど速く吸い込む
    // @param moveTimeSecond  移動にかける時間
    // @param animalPos  動物の位置
    // @return 1回当たりの移動量 //
    VECTOR3 CalcSuctionDisplacement(float moveTimeSecond,
                                    VECTOR3 animalPos, VECTOR3 plPos, float topPos, float deltaTime);
}

using namespace MyLib;