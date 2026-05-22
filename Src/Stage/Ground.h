#pragma once
#include "../Common/Object3D.h"

// 地面のメッシュとコリジョンを管理するクラス //
class CGround : public Object3D
{
public:
    //.meshのPathとmeshの大きさを渡す/
    //　初期化状態：位置(0,0,0) 中心点は5,0,5//
    /// CGround を初期化する
    /// @param mesh mesh に渡す値
    /// @param scale スケール
    CGround(const char* mesh, const VECTOR3& scale = VECTOR3(1.0f, 1.0f, 1.0f));
    /// CGround の終了処理を行う
    ~CGround();

private:
    /// 初期化する
    // bool Init(const char* filePath);
};
