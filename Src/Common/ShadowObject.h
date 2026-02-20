#pragma once
#include "../Framework/GameObject.h"
#include "../Utils/Sprite3D.h"

class Object3D;

/// <summary>
/// オブジェクトの下に影を描画するクラス
/// コンストラクタで渡したオーナーの位置・回転を参照して描画する
/// SetDrawOrder() で描画タイミングを独立して制御できる
/// </summary>
class CShadowObject : public GameObject {
public:
    CShadowObject(Object3D* pOwner, const TCHAR* imagePath, int drawOrder = -5);
    ~CShadowObject();
    void Draw() override;
    void DestroyMe() override;

private:
    Object3D*     m_pOwner;
    CSpriteImage* m_pSpriteImage;
};