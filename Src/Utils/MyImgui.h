//=============================================================================
//
//  ImGuiを使いやすくするためのヘッダーファイル            Ver 2.0     2020.11.15
//
//  このヘッダーファイルでは、./Libs/Imgui フォルダに入っている imguiシステム
//  ヘッダーを使用している。
//  また、imguiの imgui_impl_win32.h/imgui_impl_win32.cpp をカスタマイズしている。
//
//                                                  MyImgui.h
//=============================================================================
#pragma once

//#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

// ImGui
#include <shlobj.h>
#include "../../Libs/Imgui/imgui.h"
#include "../../libs/Imgui/imgui_impl_win32.h"
#include "../../libs/Imgui/imgui_impl_dx11.h"

#include "../Core/Graphics/Direct3D.h"

// ============================================================================
// namespace MyImgui
// ============================================================================
namespace MyImgui
{
    /// Imgui Init の処理を行う
    /// @param hWnd hWnd に渡す値
    /// @param m_pD3D m_pD3D に渡す値
    /// @param WidthIn 幅
    /// @param HeightIn 高さ
    void ImguiInit(HWND hWnd, CDirect3D* m_pD3D, int WidthIn, int HeightIn);
    /// Imgui Quit の処理を行う
    void ImguiQuit();
    /// Imgui New Frame の処理を行う
    void ImguiNewFrame();
    /// Imgui Render の処理を行う
    void ImguiRender();

    /// 毎フレームの状態を更新する
    void Update();
    /// Show Demo Window2 の処理を行う
    /// @param p_open p_open に渡す値
    void ShowDemoWindow2(bool* p_open = 0);

    /// Convert UTF8 To TCHAR の処理を行う
    /// @param charIn charIn に渡す値
    /// @param tcharOut tcharOut に渡す値
    void ConvertUTF8ToTCHAR(char* charIn, TCHAR* tcharOut);
    /// Convert TCHARTo UTF8 の処理を行う
    /// @param tcharIn tcharIn に渡す値
    /// @param charOut charOut に渡す値
    void ConvertTCHARToUTF8(TCHAR* tcharIn, char* charOut);
    /// Convert U8 To U16 の処理を行う
    /// @param charIn charIn に渡す値
    /// @param wcharOut wcharOut に渡す値
    void ConvertU8ToU16(char* charIn, WCHAR* wcharOut);
    /// Convert U16 To U8 の処理を行う
    /// @param wcharIn wcharIn に渡す値
    /// @param charOut charOut に渡す値
    void ConvertU16ToU8(WCHAR* wcharIn, char* charOut);
};
