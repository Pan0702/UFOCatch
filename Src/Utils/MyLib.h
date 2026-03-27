#pragma once
#include <string>

namespace MyLib
{
    /// pathからファイル名を取り出す
    /// @param path ファイルパス
    /// @return ファイル名
    std::string ChangePathToName(const std::string& path);
    /// 与えられたファイルのフォーマットが自分が欲しいものと同じかどうか
    /// @param path ファイルパス
    /// @param format 自分が欲しい形式
    /// @return 同じフォーマットならtrue
    bool IsSameFormat(const std::string& path, const std::string& format = "json");
}

