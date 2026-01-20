#pragma once
#include "../Common/Object3D.h"

// スカイボックスを表示するクラス //
class CCubeBox : public Object3D
{
public:
    // メッシュファイルのパスを指定してインスタンスを生成する
    // @param meshPath メッシュファイルのパス //
    CCubeBox(const char* meshPath);
private:
    ~CCubeBox();
    void Draw() override;
};
