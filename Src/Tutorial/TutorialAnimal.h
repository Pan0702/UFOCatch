#pragma once
#include "../Common/Object3D.h"

class CEnemyManager;

// チュートリアル用の動物クラス //
class CTutorialAnimal : public Object3D
{
public:
    // 指定位置にチュートリアル用動物を生成する
    // @param pos 生成位置 //
    CTutorialAnimal(const VECTOR3& pos);
    ~CTutorialAnimal();

    // Scoreを加算してオブジェクトをDestroy //
    void Destroy();

private:
    void Update() override;

    // 重力を適用する //
    void ApplyGravity();
    bool GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const;

    float m_velocityY = 0.0f; // Y軸方向の速度 //
    CEnemyManager* m_pEnemyManager = nullptr;
};
