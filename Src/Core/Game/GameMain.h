//=============================================================================
//
//																GameMain.h
//=============================================================================

#pragma once

#include <stdio.h>
#include <windows.h>
#include <vector>

#include "../Platform/Main.h"
#include "../Graphics/Direct3D.h"
#include "../../Framework/Shader.h"
#include "../../Utils/FbxMesh.h"
#include "../Graphics/XAudio.h"
#include "../Graphics/DInput.h"
#include "../../Utils/Sprite3D.h"
#include "../../Utils/MyImGui.h"
#include "Macro.h"

#include "../Platform/MainControl.h"

// --------------------------------------------
class CMain;
/// <summary>エンジン基盤で使う Direct3 D の情報と処理をまとめる型</summary>
class CDirect3D;
/// <summary>エンジン基盤で使う XAudio の情報と処理をまとめる型</summary>
class CXAudio;
/// <summary>エンジン基盤で使う Direct Input の情報と処理をまとめる型</summary>
class CDirectInput;
/// <summary>エンジン基盤で使う Font Texture の情報と処理をまとめる型</summary>
class CFontTexture;
/// <summary>Direct3D用シェーダと定数バッファを管理するクラス</summary>
class CShader;
/// <summary>エンジン基盤で使う Fbx Mesh Ctrl の情報と処理をまとめる型</summary>
class CFbxMeshCtrl;

// --------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
    CMain* m_pMain;
    CDirect3D* m_pD3D;
    CXAudio* m_pXAudio;
    CDirectInput* m_pDI;
    CFontTexture* m_pFont;
    CShader* m_pShader;
    CFbxMeshCtrl* m_pFbxMeshCtrl;

    VECTOR3 m_vEyePt;
    VECTOR3 m_vLookatPt;
    MATRIX4X4 m_mView;
    MATRIX4X4 m_mProj;
    VECTOR3 m_vLightDir;
    VECTOR4 m_vLightIntensity;

public:
    /// CGameMain を初期化する
    /// @param pMain pMain に渡す値
    CGameMain(CMain* pMain);
    /// CGameMain の終了処理を行う
    ~CGameMain();

    /// 初期化する
    /// @return 処理結果
    HRESULT Init();
    /// 毎フレームの状態を更新する
    void Update();
    /// 描画する
    void Draw();
    /// 終了処理を行う
    void Quit();

    /// Game Main の処理を行う
    void GameMain();
    /// Screen Mode を切り替える
    /// @param nMode nMode に渡す値
    /// @return 処理結果
    HRESULT ChangeScreenMode(int nMode = -1); // -- 2020.1.15

    /// Window Name を設定する
    /// @param name 名前
    void SetWindowName(const char* name);

    /// Light Intensity を設定する
    /// @param r r に渡す値
    /// @param g g に渡す値
    /// @param b b に渡す値
    /// @param a a に渡す値
    void SetLightIntensity(float r, float g, float b, float a = 1.0f);
    /// Light Intensity を設定する
    /// @param intensity intensity に渡す値
    void SetLightIntensity(VECTOR4 intensity);
    /// Light Intensity を取得する
    /// @return 4次元ベクトル
    VECTOR4 GetLightIntensity() const { return m_vLightIntensity; }
};

/// Game Device を返す
/// @return 対象のポインタ
CGameMain* GameDevice();
