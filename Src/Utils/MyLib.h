#pragma once
#include <string>

namespace MyLib
{
    /// Path To Name を切り替える
    /// @param path パス
    /// @return 取得した文字列
    std::string ChangePathToName(const std::string& path);
    /// Same Format を判定する
    /// @param path パス
    /// @param format format に渡す値
    /// @return 成功または条件を満たす場合 true
    bool IsSameFormat(const std::string& path, const std::string& format = "json");
}
