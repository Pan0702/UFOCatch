#pragma once
#include "../05_CommonFile/Object3D.h"

class CGround;

// チュートリアル用の動物クラス //
class CTutorialAnimal : public Object3D
{
public:
    // 指定位置にチュートリアル用動物を生成する
    // @param pos 生成位置 //
    CTutorialAnimal(const VECTOR3& pos);
    //　Scoreを足してオブジェクトをDestroy//
    void Destroy();

private:
    ~CTutorialAnimal();

    void Update() override;

    // 重力を適用する //
    void ApplyGravity();

    float m_velocityY = 0.0f;      // Y軸方向の速度 //
    CGround* m_pGround = nullptr;  // 地面オブジェクトへのポインタ //
};
