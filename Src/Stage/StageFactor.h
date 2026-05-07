#pragma once
#include "../Framework/GameObject.h"

// ステージのオブジェクトを生成するファクトリークラス //
class CStageFactor : public GameObject
{
public:
    CStageFactor();
    CStageFactor(const char* path);
    static void SpawnObjects(float sizeX, float sizeZ, int num);
    /// オブジェクトを出現させる。
    /// @param path ファイルパス
    /// @param size オブジェクトのサイズ
    /// @param num 出現させるオブジェクトの数
    static void SpawnObjects(const std::string& path, const VECTOR2& size = VECTOR2(51.0f, 51.0f), int num = 90);
};
