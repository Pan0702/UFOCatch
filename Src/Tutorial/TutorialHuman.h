#pragma once
#include "TutorialAnimal.h"

class CFunShape;

// チュートリアル用の人間クラス //
class CTutorialHuman : public Object3D
{
public:
    // 指定位置にチュートリアル用人間を生成する
    // @param pos 生成位置 //
    CTutorialHuman(const VECTOR3& pos);
    ~CTutorialHuman();
    void Update() override;

private:
    void UpdateVisionShape() const;

    bool m_inSight; // プレイヤーが視界内にいるかどうか //
    CFunShape* m_pFunShape = nullptr;
};
