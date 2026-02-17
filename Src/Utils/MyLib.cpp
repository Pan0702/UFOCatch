#include "MyLib.h"
#include "../Framework/sceneManager.h"

////////////////////
// 引き寄せるための移動量を計算する
// 高さの差が大きいほど遅く、近いほど速く吸い込む
// @param moveTimeSecond  移動にかける時間
// @param animalPos  動物の位置
// @return 1回当たりの移動量
////////////////////

const VECTOR3 MyLib::CalcSuctionDisplacement(const float& moveTimeSecond, const VECTOR3& animalPos,
                                             const VECTOR3& plPos, const float topPos)
{
    const float heightDiff = topPos - animalPos.y;
    const float progress = 1.0f - (heightDiff / topPos);
    const float eased = (std::max)(0.0f, (std::min)(1.0f, progress));
    constexpr float minSpeed = 0.4f;                                  
    constexpr float maxSpeed = 1.8f;  
    const float heightSpeedMultiplier = minSpeed + (maxSpeed - minSpeed) * eased;

    const float projectionFactorY0 = avoidZero((0 - animalPos.y) / (animalPos.y - topPos));
    VECTOR3 targetPointOnPlane =
        VECTOR3(animalPos.x + projectionFactorY0 * (animalPos.x - plPos.x), 0,
                animalPos.z + projectionFactorY0 * (animalPos.z - plPos.z));
    VECTOR3 pullVectorToTarget = plPos - targetPointOnPlane;

    const VECTOR3 suctionDisplacementPerFrame = pullVectorToTarget / moveTimeSecond * heightSpeedMultiplier;
    return suctionDisplacementPerFrame * SceneManager::DeltaTime();
}
