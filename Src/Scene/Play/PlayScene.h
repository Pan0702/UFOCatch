#pragma once
#include "../../Framework/SceneBase.h"

// メインのプレイシーン //
class PlayScene : public SceneBase
{
public:
    /// PlayScene を初期化する
    PlayScene();
    /// PlayScene の終了処理を行う
    ~PlayScene();
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;
    // リザルトシーンへ遷移する //
    /// Result Scene を切り替える
    void ChangeResultScene();

private:
    //4分木デバッグ
    /// QTree Debug の処理を行う
    void QTreeDebug();
};
