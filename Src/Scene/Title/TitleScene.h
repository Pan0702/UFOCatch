#pragma once
#include "../../Framework/SceneBase.h"
#include "../../Utils/WipeAnimator.h"

// ゲーム起動時のタイトルシーン //
class TitleScene : public SceneBase
{
public:
    /// TitleScene を初期化する
    TitleScene();
    /// TitleScene の終了処理を行う
    ~TitleScene();

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;
};
