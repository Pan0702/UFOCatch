#pragma once
#include <string>

/// <summary>汎用ユーティリティで使う Json Reader の情報と処理をまとめる型</summary>
class JsonReader
{
public:
    /// Model Data を読み込む
    /// @param path パス
    static void ImportModelData(const std::string& path);
};
