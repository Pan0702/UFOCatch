#pragma once
#include <string>

namespace Platform
{
    /// <summary>
    /// Windowsのファイルオープンダイアログを表示し、選択されたファイルのパスを返す
    /// </summary>
    /// <param name="filter">ファイルフィルター（例: L"*.mesh"）</param>
    /// <returns>選択されたファイルのフルパス（UTF-8）。キャンセル時は空文字列</returns>
    std::string OpenFileDialog(const wchar_t* filter = L"All Files (*.*)\0*.*\0");

    /// <summary>
    /// Windowsのファイルセーブダイアログを表示し、選択されたファイルのパスを返す
    /// </summary>
    /// <param name="filter">ファイルフィルター（例: L"*.mesh"）</param>
    /// <returns>選択されたファイルのフルパス（UTF-8）。キャンセル時は空文字列</returns>
    std::string SaveFileDialog(const wchar_t* filter = L"All Files (*.*)\0*.*\0", const wchar_t* initial_dir = nullptr);
}

