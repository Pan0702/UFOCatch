//=============================================================================
//
//	ImGuiを使いやすくするためのヘッダファイル            Ver 2.0     2020.11.15
//
//	このヘッダファイルでは、./Libs/Imgui フォルダに入っている imguiシステムの
//	ヘッダを使用している
//	また、imguiのimgui_impl_win32.h/imgui_impl_win32.cppはカスタマイズしてある。
//
//																	MyImgui.h
//=============================================================================
#pragma once

//#define _CRT_SECURE_NO_WARNINGS

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

// ImGui
#include <shlobj.h>
#include "../libs/Imgui/imgui.h"
#include "../libs/Imgui/imgui_impl_win32.h"
#include "../libs/Imgui/imgui_impl_dx11.h"

#include "Direct3D.h"

// ============================================================================
// 一般関数
// namespace MyImgui
// ============================================================================
namespace MyImgui
{
	void ImguiInit(HWND hWnd, CDirect3D* m_pD3D, int WidthIn, int HeightIn);
	void ImguiQuit();
	void ImguiNewFrame();
	void ImguiRender();

	void Update();
	void ShowDemoWindow2(bool* p_open = 0);

	void ConvertUTF8ToTCHAR(char* charIn, TCHAR* tcharOut);
	void ConvertTCHARToUTF8(TCHAR* tcharIn, char* charOut);
	void ConvertU8ToU16(char* charIn, WCHAR* wcharOut);
	void ConvertU16ToU8(WCHAR* wcharIn, char* charOut);
};