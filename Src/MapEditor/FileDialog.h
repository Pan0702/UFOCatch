#pragma once
#include <string>

namespace Platform
{
    /// Open File Dialog を返す
    /// @param filter filter に渡す値
    /// @return 取得した文字列
    std::string OpenFileDialog(const wchar_t* filter = L"All Files (*.*)\0*.*\0");

    /// File Dialog を保存する
    /// @param filter filter に渡す値
    /// @param initial_dir initial_dir に渡す値
    /// @return 取得した文字列
    std::string SaveFileDialog(const wchar_t* filter = L"All Files (*.*)\0*.*\0", const wchar_t* initial_dir = nullptr);
}
