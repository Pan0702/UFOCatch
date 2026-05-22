// ========================================================================================
//
//
//
//
//
//
//                                                                              Shader.h
// ========================================================================================
#pragma once

#include <stdio.h>
#include <windows.h>

#include "../Core/Platform/Main.h"
#include "../Core/Graphics/Direct3D.h"


/// <summary>ゲーム共通基盤で使う CONSTANT_BUFFER_WVLED の情報と処理をまとめる型</summary>
struct CONSTANT_BUFFER_WVLED
{
    MATRIX4X4 mW;
    MATRIX4X4 mWVP;
    VECTOR4 vLightDir; // ライト方向
    VECTOR4 vEyePos; // 視点座標
    VECTOR4 vDiffuse;
    VECTOR4 vDrawInfo;

    CONSTANT_BUFFER_WVLED()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_WVLED));
    }
};

/// <summary>ゲーム共通基盤で使う CONSTANT_BUFFER_DISPLACE の情報と処理をまとめる型</summary>
struct CONSTANT_BUFFER_DISPLACE
{
    VECTOR3 vEyePosInv;
    float fMinDistance;
    float fMaxDistance;
    int iMaxDevide;
    VECTOR2 vHeight;
    VECTOR4 vWaveMove;
    VECTOR4 vSpecular;

    CONSTANT_BUFFER_DISPLACE()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_DISPLACE));
    }
};

struct CONSTANT_BUFFER_EFFECT
{
    MATRIX4X4 mWVP;
    MATRIX4X4 mW;
    MATRIX4X4 mV;
    MATRIX4X4 mP;
    VECTOR2 vUVOffset;
    VECTOR2 vUVScale;
    float fAlpha;
    float fSize;
    VECTOR2 Dummy;

    CONSTANT_BUFFER_EFFECT()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_EFFECT));
    }
};


struct CONSTANT_BUFFER_SPRITE
{
    MATRIX4X4 mWVP;
    MATRIX4X4 mW;
    float ViewPortWidth;
    float ViewPortHeight;
    VECTOR2 vUVOffset;
    VECTOR4 vColor;
    VECTOR4 vMatInfo;

    CONSTANT_BUFFER_SPRITE()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_SPRITE));
    }
};

/// <summary>ゲーム共通基盤で使う CONSTANT_BUFFER_ARC の情報と処理をまとめる型</summary>
struct CONSTANT_BUFFER_ARC
{
    float startAngle; // 開始角度（ラジアン、0=12時）
    float arcSpan; // 弧の長さ（正=CW、負=CCW）
    float innerRadius; // 内半径（0〜0.5）
    float padding;
    VECTOR2 uvMin; // UV範囲の最小値
    VECTOR2 uvMax; // UV範囲の最大値

    CONSTANT_BUFFER_ARC()
    {
        ZeroMemory(this,
                   sizeof(CONSTANT_BUFFER_ARC));
    }
};


/// <summary>ゲーム共通基盤で使う CONSTANT_BUFFER_MATERIAL の情報と処理をまとめる型</summary>
struct CONSTANT_BUFFER_MATERIAL
{
    VECTOR4 vMatDuffuse;
    VECTOR4 vMatSpecular;
};

//
// CShaderクラス
//
class CShader
{
public:
    // Direct3D11
    CDirect3D* m_pD3D;

    // シェーダーオブジェクト
    // 通常用のシンプルなシェーダー
    ID3D11InputLayout* m_pSimple_VertexLayout;
    ID3D11VertexShader* m_pSimple_VS;
    ID3D11PixelShader* m_pSimple_PS;

    // 3Dスプライト用のシェーダー
    ID3D11InputLayout* m_pSprite3D_VertexLayout;
    ID3D11VertexShader* m_pSprite3D_VS;
    ID3D11PixelShader* m_pSprite3D_PS;
    ID3D11VertexShader* m_pSprite3D_VS_BILL;

    ID3D11InputLayout* m_pDisplaceWave_VertexLayout;
    ID3D11VertexShader* m_pDisplaceWave_VS;
    ID3D11HullShader* m_pDisplaceWave_HS;
    ID3D11DomainShader* m_pDisplaceWave_DS;
    ID3D11PixelShader* m_pDisplaceWave_PS;

    ID3D11VertexShader* m_pDisplaceSkinMesh_VS;
    ID3D11HullShader* m_pDisplaceSkinMesh_HS;
    ID3D11DomainShader* m_pDisplaceSkinMesh_DS;
    ID3D11PixelShader* m_pDisplaceSkinMesh_PS;

    ID3D11VertexShader* m_pDisplaceStaticMesh_VS;
    ID3D11HullShader* m_pDisplaceStaticMesh_HS;
    ID3D11DomainShader* m_pDisplaceStaticMesh_DS;
    ID3D11PixelShader* m_pDisplaceStaticMesh_PS;

    // エフェクト用のシェーダー
    ID3D11InputLayout* m_pEffect3D_VertexLayout;
    ID3D11VertexShader* m_pEffect3D_VS_POINT;
    ID3D11GeometryShader* m_pEffect3D_GS_POINT;
    ID3D11PixelShader* m_pEffect3D_PS;
    ID3D11InputLayout* m_pEffect3D_VertexLayout_BILL;
    ID3D11VertexShader* m_pEffect3D_VS_BILL;
    ID3D11VertexShader* m_pEffect3D_VS_BILLMESH; // -- 2019.7.17

    ID3D11InputLayout* m_pFbxStaticMesh_VertexLayout;
    ID3D11VertexShader* m_pFbxStaticMesh_VS;
    ID3D11PixelShader* m_pFbxStaticMesh_PS;

    ID3D11InputLayout* m_pFbxSkinMesh_VertexLayout;
    ID3D11VertexShader* m_pFbxSkinMesh_VS;
    ID3D11PixelShader* m_pFbxSkinMesh_PS;
    ID3D11Buffer* m_pConstantBufferArc;


    // 定数バッファ  ------------------------------------------

    ID3D11Buffer* m_pConstantBufferDisplace;

    // エフェクト用の定数バッファ
    ID3D11Buffer* m_pConstantBufferEffect;

    // 3Dスプライト用の定数バッファ
    ID3D11Buffer* m_pConstantBufferSprite3D;

    ID3D11Buffer* m_pConstantBufferWVLED; // -- 2020.1.24

    ID3D11Buffer* m_pConstantBufferBone2;

    ID3D11Buffer* m_pConstantBufferMaterial; // -- 2020.12.15


public:
    /// Shader を初期化する
    /// @return 処理結果
    HRESULT InitShader();
    /// Shader Simple を初期化する
    /// @return 処理結果
    HRESULT InitShaderSimple();
    /// Shader Sprite を初期化する
    /// @return 処理結果
    HRESULT InitShaderSprite();

    /// Shader Fbx を初期化する
    /// @return 処理結果
    HRESULT InitShaderFbx();
    /// Shader Displace を初期化する
    /// @return 処理結果
    HRESULT InitShaderDisplace();
    /// Shader Effect を初期化する
    /// @return 処理結果
    HRESULT InitShaderEffect();
    /// Shader Constant を初期化する
    /// @return 処理結果
    HRESULT InitShaderConstant();

    /// Shader を作成する
    /// @param ppShader ppShader に渡す値
    /// @param numElements numElements に渡す値
    /// @param ppInputLayout ppInputLayout に渡す値
    /// @return 処理結果
    HRESULT MakeShader(const TCHAR ProfileName[], const TCHAR FileName[], void** ppShader,
                       D3D11_INPUT_ELEMENT_DESC Fluid_layout[] = nullptr, UINT numElements = 0,
                       ID3D11InputLayout** ppInputLayout = nullptr);
    /// Constant Buffer を作成する
    /// @param size サイズ
    /// @param ppConstantBuffer ppConstantBuffer に渡す値
    /// @return 処理結果
    HRESULT MakeConstantBuffer(UINT size, ID3D11Buffer** ppConstantBuffer);

    /// CShader を初期化する
    /// @param pD3D pD3D に渡す値
    CShader(CDirect3D* pD3D);
    /// CShader の終了処理を行う
    ~CShader();
};
