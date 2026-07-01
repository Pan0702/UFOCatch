#pragma once
#include "../Common/Object3D.h"

/// <summary>ステージエディタで使う Back Graund の情報と処理をまとめる型</summary>
class BackGraund : public Object3D
{
private:
    CFbxMesh* mesh;

public:
    /// BackGraund を初期化する
    BackGraund();
};
