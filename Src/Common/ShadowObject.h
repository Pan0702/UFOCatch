#pragma once
#include "../Framework/GameObject.h"
#include "../Utils/Sprite3D.h"

/// <summary>Transform、メッシュ、アニメーション、コライダーを持つ3Dオブジェクト基底クラス</summary>
class Object3D;

/// <summary>
/// SetDrawOrder() で描画タイミングを独立して制御できる
/// </summary>
class CShadowObject : public GameObject
{
public:
    /// CShadowObject を初期化する
    /// @param pOwner pOwner に渡す値
    /// @param imagePath パス
    /// @param drawOrder drawOrder に渡す値
    CShadowObject(Object3D* pOwner, const TCHAR* imagePath, int drawOrder = -5);
    /// CShadowObject の終了処理を行う
    ~CShadowObject();
    /// 開始する
    void Start() override;
    /// 描画する
    void Draw() override;
    /// Me を破棄する
    void DestroyMe() override;

private:
    Object3D* m_pOwner;
    CSpriteImage* m_pSpriteImage;
};
