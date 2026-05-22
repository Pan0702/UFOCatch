#pragma once
#include "../../Framework/SceneBase.h"
#include "../../Utils/Sprite3D.h"

/// <summary>シーンで使う OIScene の情報と処理をまとめる型</summary>
class OIScene : public SceneBase
{
public:
    /// OIScene を初期化する
    OIScene();

private:
    /// 描画する
    void Draw() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    std::unique_ptr<CSpriteImage> m_pSpriteImage;
};
