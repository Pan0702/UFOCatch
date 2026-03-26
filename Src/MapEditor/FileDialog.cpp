#include "FileDialog.h"
#include <windows.h>
#include <shobjidl.h>

namespace Platform
{
    // ファイルオープンダイアログを表示し、選択されたファイルのパスをUTF-8で返す
    std::string OpenFileDialog(const wchar_t* filter)
    {
        std::string result;

        IFileOpenDialog* file_open = nullptr;

        HRESULT hr = CoCreateInstance(
            CLSID_FileOpenDialog,
            nullptr,
            CLSCTX_ALL,
            IID_IFileOpenDialog,
            reinterpret_cast<void**>(&file_open));

        if (SUCCEEDED(hr))
        {
            if (filter)
            {
                COMDLG_FILTERSPEC spec[] =
                {
                    {L"Files", filter}
                };
                file_open->SetFileTypes(1, spec);
            }

            hr = file_open->Show(nullptr);

            if (SUCCEEDED(hr))
            {
                IShellItem* item = nullptr;
                hr = file_open->GetResult(&item);

                if (SUCCEEDED(hr))
                {
                    PWSTR file_path = nullptr;
                    hr = item->GetDisplayName(SIGDN_FILESYSPATH, &file_path);

                    if (SUCCEEDED(hr))
                    {
                        char buffer[MAX_PATH];
                        WideCharToMultiByte(
                            CP_UTF8, 0,
                            file_path, -1,
                            buffer, MAX_PATH,
                            nullptr, nullptr);

                        result = buffer;
                        CoTaskMemFree(file_path);
                    }

                    item->Release();
                }
            }

            file_open->Release();
        }

        return result;
    }

    // Windowsのファイルセーブダイアログを表示し、選択されたファイルのパスを返す
    std::string SaveFileDialog(const wchar_t* filter, const wchar_t* initial_dir)
    {
        std::string result;

        IFileSaveDialog* file_save = nullptr;

        HRESULT hr = CoCreateInstance(
            CLSID_FileSaveDialog,
            nullptr,
            CLSCTX_ALL,
            IID_IFileSaveDialog,
            reinterpret_cast<void**>(&file_save));

        if (SUCCEEDED(hr))
        {
            if (filter)
            {
                COMDLG_FILTERSPEC spec[] =
                {
                    {L"Files", filter}
                };
                file_save->SetFileTypes(1, spec);
            }
            // ファイル名をjsonで保存する
            file_save->SetDefaultExtension(L"json");

            if (initial_dir)
            {
                IShellItem* folder = nullptr;
                if (SUCCEEDED(SHCreateItemFromParsingName(initial_dir, nullptr, IID_PPV_ARGS(&folder))))
                {
                    file_save->SetFolder(folder);
                    folder->Release();
                }
            }
            hr = file_save->Show(nullptr);

            if (SUCCEEDED(hr))
            {
                IShellItem* item = nullptr;
                hr = file_save->GetResult(&item);

                if (SUCCEEDED(hr))
                {
                    PWSTR file_path = nullptr;
                    hr = item->GetDisplayName(SIGDN_FILESYSPATH, &file_path);

                    if (SUCCEEDED(hr))
                    {
                        char buffer[MAX_PATH];
                        WideCharToMultiByte(
                            CP_UTF8, 0,
                            file_path, -1,
                            buffer, MAX_PATH,
                            nullptr, nullptr);

                        result = buffer;
                        CoTaskMemFree(file_path);
                    }

                    item->Release();
                }
            }

            file_save->Release();
        }
        return result;
    }
}
