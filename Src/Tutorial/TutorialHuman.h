#pragma once
#include "TutorialAnimal.h"

// チュートリアル用の人間クラス //
class CTutorialHuman : public Object3D
{
public:
    // 指定位置にチュートリアル用人間を生成する
    // @param pos 生成位置 //
    CTutorialHuman(const VECTOR3& pos);
    ~CTutorialHuman();

private:
    void Update() override;

    bool m_inSight;  // プレイヤーが視界内にいるかどうか //
};