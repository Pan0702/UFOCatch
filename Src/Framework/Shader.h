// ========================================================================================
//
// シェーダーの処理                                               ver 3.0        2021.1.11
//
//   シェーダーオブジェクトの読み込みとシェーダーの作成
//   インプットレイアウトの作成
//   定数バッファの作成
//
//
//    登録されているシェーダー
//
//     ・シンプルなシェーダー                          InitShaderSimple();
//     ・スプライト用のシェーダー                      InitShaderSprite();
//     ・FBXStaticMesh/FBXSkinMesh用のシェーダー       InitShaderFbx();
//     ・ディスプレイスメントマッピング用のシェーダー  InitShaderDisplace();
//     ・エフェクト用のシェーダー                      InitShaderEffect();
//
//                                                                              Shader.h
// ========================================================================================
#pragma once

// ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "../Core/Platform/Main.h"
#include "../Core/Graphics/Direct3D.h"

// シェーダー用バッファ構造体の定義

// メッシュシェーダー用の定数バッファ構造体
// （ワールド行列から投影行列、ライト、カラー情報を保持）  // -- 2020.1.24
struct CONSTANT_BUFFER_WVLED
{
    MATRIX4X4  mW;             // ワールド行列
    MATRIX4X4  mWVP;           // ワールド・ビュー・投影変換行列
    VECTOR4    vLightDir;      // ライト方向
    VECTOR4    vEyePos;        // 視点座標
    VECTOR4    vDiffuse;       // ディフューズ色   
    VECTOR4    vDrawInfo;      // 描画関連情報(現在は未使用)   // -- 2020.12.15
    CONSTANT_BUFFER_WVLED()
    {
       ZeroMemory(this, sizeof(CONSTANT_BUFFER_WVLED));
    }
};

// ディスプレイスメントマッピング用の各種データを渡すための構造体  // -- 2020.1.24
struct CONSTANT_BUFFER_DISPLACE
{
    VECTOR3    vEyePosInv;    // モデル座標系から見た視点位置
    float      fMinDistance;  // テッセレーションの最小距離
    float      fMaxDistance;  // テッセレーションの最大距離
    int        iMaxDevide;    // 最大分割数
    VECTOR2    vHeight;       // ディスプレイスメントマッピングの高さ係数
    VECTOR4    vWaveMove;     // 波の移動量（波シェーダー用）
    VECTOR4    vSpecular;     // スペキュラー情報（波シェーダー用）
    CONSTANT_BUFFER_DISPLACE()
    {
       ZeroMemory(this, sizeof(CONSTANT_BUFFER_DISPLACE));
    }
};

// エフェクト用の定数バッファ構造体   //  2017.8.25
struct CONSTANT_BUFFER_EFFECT
{
    MATRIX4X4  mWVP;       // ワールド・ビュー・投影変換行列
    MATRIX4X4  mW;         // ワールド行列
    MATRIX4X4  mV;         // ビュー行列
    MATRIX4X4  mP;         // 投影行列
    VECTOR2    vUVOffset;  // テクスチャ座標のオフセット
    VECTOR2    vUVScale;   // テクスチャ座標のスケール  // -- 2019.7.17
    float      fAlpha;     // 透明度
    float      fSize;      // パーティクルの大きさ  // -- 2018.8.23
    VECTOR2    Dummy;      // パディング用ダミーベクトル       // -- 2019.7.17
    CONSTANT_BUFFER_EFFECT()
    {
       ZeroMemory(this, sizeof(CONSTANT_BUFFER_EFFECT));
    }
};


// 3Dスプライトシェーダー用の定数バッファ構造体
struct CONSTANT_BUFFER_SPRITE
{
    MATRIX4X4  mWVP;
    MATRIX4X4  mW;
    float      ViewPortWidth;
    float      ViewPortHeight;
    VECTOR2    vUVOffset;
    VECTOR4    vColor;         // カラー（透明度を含む）
    VECTOR4    vMatInfo;       // マテリアル関連情報（x:テクスチャ有無など）。DrawRect()、DrawLine()で使用。
    CONSTANT_BUFFER_SPRITE()
    {
       ZeroMemory(this, sizeof(CONSTANT_BUFFER_SPRITE));
    }
};

// マテリアル情報構造体                      // -- 2020.12.15
struct CONSTANT_BUFFER_MATERIAL
{
    VECTOR4    vMatDuffuse;
    VECTOR4    vMatSpecular;
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

    // ディスプレイスメントマッピング(波)用のシェーダー
    ID3D11InputLayout* m_pDisplaceWave_VertexLayout;
    ID3D11VertexShader* m_pDisplaceWave_VS;
    ID3D11HullShader* m_pDisplaceWave_HS;
    ID3D11DomainShader* m_pDisplaceWave_DS;
    ID3D11PixelShader* m_pDisplaceWave_PS;

    // ディスプレイスメントマッピング(スキンメッシュ)用のシェーダー
    ID3D11VertexShader* m_pDisplaceSkinMesh_VS;
    ID3D11HullShader* m_pDisplaceSkinMesh_HS;
    ID3D11DomainShader* m_pDisplaceSkinMesh_DS;
    ID3D11PixelShader* m_pDisplaceSkinMesh_PS;

    // ディスプレイスメントマッピング(スタティックメッシュ)用のシェーダー
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
    ID3D11VertexShader* m_pEffect3D_VS_BILLMESH;  // -- 2019.7.17

    // Fbxモデル：スタティックメッシュ用のシェーダー
    // (Normalマッピング対応)
    ID3D11InputLayout* m_pFbxStaticMesh_VertexLayout;
    ID3D11VertexShader* m_pFbxStaticMesh_VS;
    ID3D11PixelShader* m_pFbxStaticMesh_PS;

    // Fbxモデル：スキンメッシュ用のシェーダー
    // (Normalマッピング対応)
    ID3D11InputLayout* m_pFbxSkinMesh_VertexLayout;
    ID3D11VertexShader* m_pFbxSkinMesh_VS;
    ID3D11PixelShader* m_pFbxSkinMesh_PS;


    // 定数バッファ  ------------------------------------------

    // ディスプレイスメントマッピング用の定数バッファ   // -- 2020.1.24
    ID3D11Buffer* m_pConstantBufferDisplace;

    // エフェクト用の定数バッファ
    ID3D11Buffer* m_pConstantBufferEffect;

    // 3Dスプライト用の定数バッファ
    ID3D11Buffer* m_pConstantBufferSprite3D;

    // メッシュの変換行列・カラー情報用定数バッファ
    ID3D11Buffer* m_pConstantBufferWVLED;    // -- 2020.1.24

    // ボーン行列転送用の定数バッファ
    ID3D11Buffer* m_pConstantBufferBone2;

    // マテリアル情報用定数バッファ
    ID3D11Buffer* m_pConstantBufferMaterial;    // -- 2020.12.15


public:
    HRESULT InitShader();
    HRESULT InitShaderSimple();
    HRESULT InitShaderSprite();

    HRESULT InitShaderFbx();
    HRESULT InitShaderDisplace();
    HRESULT InitShaderEffect();
    HRESULT InitShaderConstant();

    HRESULT MakeShader(const TCHAR ProfileName[], const TCHAR FileName[], void** ppShader, D3D11_INPUT_ELEMENT_DESC Fluid_layout[] = nullptr, UINT numElements = 0, ID3D11InputLayout** ppInputLayout = nullptr);
    HRESULT MakeConstantBuffer(UINT size, ID3D11Buffer** ppConstantBuffer);

    CShader(CDirect3D* pD3D);
    ~CShader();

};