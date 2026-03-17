#pragma once
#include <string>

class Import
{
public:
    /// <summary>
    /// JSONファイルを読み込み、記録されたモデルをステージに復元する。
    /// 未ロードのモデルは自動的にロードしてボタンにも追加する。
    /// </summary>
    /// <param name="path">インポートするJSONファイルのパス</param>
    static void ImportFromFile(const std::string& path);
};