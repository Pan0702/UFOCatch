// -----------------------------------------------------------------------------------------------------------  // -- 2020.8.7
// 
// ImGui Windows API 用のヘッダファイル
// 
//   日本語入力に対応するためと画面の拡縮・フルスクリーンに対応するためにカスタマイズしてある
//   カスタマイズ箇所には、「// -- 2020.8.7」の印がつけてある。
//   また、変更前の箇所には、「// 元の内容」の印がつけてある。
//   
// -----------------------------------------------------------------------------------------------------------

// dear imgui: Platform Binding for Windows (standard windows API for 32 and 64 bits applications)
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)

// Implemented features:
//  [X] Platform: Clipboard support (for Win32 this is actually part of core imgui)
//  [X] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.
//  [X] Platform: Keyboard arrays indexed using VK_* Virtual Key Codes, e.g. ImGui::IsKeyPressed(VK_SPACE).
//  [X] Platform: Gamepad support. Enabled with 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad'.

#pragma once

// 画面の拡縮処理の追加に合わせて画面の初期サイズを得るように関数を修正       // -- 2020.8.7
IMGUI_IMPL_API bool     ImGui_ImplWin32_Init(void* hwnd, int WidthIn, int HeightIn);    // -- 2020.8.7
//IMGUI_IMPL_API bool     ImGui_ImplWin32_Init(void* hwnd);       // 元の内容

IMGUI_IMPL_API void     ImGui_ImplWin32_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplWin32_NewFrame();

// Handler for Win32 messages, update mouse/keyboard data.
// You may or not need this for your implementation, but it can serve as reference for handling inputs.
// Intentionally commented out to avoid dragging dependencies on <windows.h> types. You can COPY this line into your .cpp code instead.
/*
IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
*/
