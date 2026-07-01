#pragma once
#include "../Common/Object3D.h"

// スカイボックスを表示するクラス //
class CCubeBox : public Object3D
{
public:
    // メッシュファイルのパスを指定してインスタンスを生成する
    // @param meshPath メッシュファイルのパス //
    /// CCubeBox を初期化する
    /// @param meshPath パス
    CCubeBox(const char* meshPath);
    /// CCubeBox の終了処理を行う
    ~CCubeBox();

private:
    /// 描画する
    void Draw() override;
};
