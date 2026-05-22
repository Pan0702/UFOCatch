#pragma once
#include "../Framework/GameObject.h"

// ステージのオブジェクトを生成するファクトリークラス //
class CStageFactor : public GameObject
{
public:
    /// CStageFactor を初期化する
    CStageFactor();
    /// CStageFactor を初期化する
    /// @param path パス
    CStageFactor(const char* path);
    /// Spawn Objects の処理を行う
    /// @param sizeX サイズ
    /// @param sizeZ サイズ
    /// @param num num に渡す値
    static void SpawnObjects(float sizeX, float sizeZ, int num);
    /// Spawn Objects の処理を行う
    /// @param path パス
    /// @param size サイズ
    /// @param num num に渡す値
    static void SpawnObjects(const std::string& path, const VECTOR2& size = VECTOR2(51.0f, 51.0f), int num = 90);
};
