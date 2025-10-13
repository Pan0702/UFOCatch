//=============================================================================
//
//	ImGui���g���₷�����邽�߂̃w�b�_�t�@�C��            Ver 2.0     2020.11.15
//
//	���̃w�b�_�t�@�C���ł́A./Libs/Imgui �t�H���_�ɓ����Ă��� imgui�V�X�e����
//	�w�b�_���g�p���Ă���
//	�܂��Aimgui��imgui_impl_win32.h/imgui_impl_win32.cpp�̓J�X�^�}�C�Y���Ă���B
//
//																	MyImgui.h
//=============================================================================
#pragma once

//#define _CRT_SECURE_NO_WARNINGS

//�x����\��ni
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)

// ImGui
#include <shlobj.h>
#include "../../Libs/Imgui/imgui.h"
#include "../../libs/Imgui/imgui_impl_win32.h"
#include "../../libs/Imgui/imgui_impl_dx11.h"

#include "../02_DirectX/Direct3D.h"

// ============================================================================
// ��ʊ֐�
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