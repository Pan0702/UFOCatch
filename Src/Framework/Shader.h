// ========================================================================================
//
// 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ蜃ｦ逅・                                              ver 3.0        2021.1.11
//
//   繧ｷ繧ｧ繝ｼ繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医・隱ｭ縺ｿ霎ｼ縺ｿ縺ｨ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・
//   繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医・菴懈・
//   螳壽焚繝舌ャ繝輔ぃ縺ｮ菴懈・
//
//
//    逋ｻ骭ｲ縺輔ｌ縺ｦ縺・ｋ繧ｷ繧ｧ繝ｼ繝繝ｼ
//
//     繝ｻ繧ｷ繝ｳ繝励Ν縺ｪ繧ｷ繧ｧ繝ｼ繝繝ｼ                          InitShaderSimple();
//     繝ｻ繧ｹ繝励Λ繧､繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ                      InitShaderSprite();
//     繝ｻFBXStaticMesh/FBXSkinMesh逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ       InitShaderFbx();
//     繝ｻ繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ  InitShaderDisplace();
//     繝ｻ繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ                      InitShaderEffect();
//
//                                                                              Shader.h
// ========================================================================================
#pragma once

// 繝倥ャ繝繝ｼ繝輔ぃ繧､繝ｫ縺ｮ繧､繝ｳ繧ｯ繝ｫ繝ｼ繝・
#include <stdio.h>
#include <windows.h>

#include "../Core/Platform/Main.h"
#include "../Core/Graphics/Direct3D.h"

// 繧ｷ繧ｧ繝ｼ繝繝ｼ逕ｨ繝舌ャ繝輔ぃ讒矩菴薙・螳夂ｾｩ

// 繝｡繝・す繝･繧ｷ繧ｧ繝ｼ繝繝ｼ逕ｨ縺ｮ螳壽焚繝舌ャ繝輔ぃ讒矩菴・
// ・医Ρ繝ｼ繝ｫ繝芽｡悟・縺九ｉ謚募ｽｱ陦悟・縲√Λ繧､繝医√き繝ｩ繝ｼ諠・ｱ繧剃ｿ晄戟・・ // -- 2020.1.24
struct CONSTANT_BUFFER_WVLED
{
    MATRIX4X4 mW; // 繝ｯ繝ｼ繝ｫ繝芽｡悟・
    MATRIX4X4 mWVP; // 繝ｯ繝ｼ繝ｫ繝峨・繝薙Η繝ｼ繝ｻ謚募ｽｱ螟画鋤陦悟・
    VECTOR4 vLightDir; // 繝ｩ繧､繝域婿蜷・
    VECTOR4 vEyePos; // 隕也せ蠎ｧ讓・
    VECTOR4 vDiffuse; // 繝・ぅ繝輔Η繝ｼ繧ｺ濶ｲ   
    VECTOR4 vDrawInfo; // 謠冗判髢｢騾｣諠・ｱ(迴ｾ蝨ｨ縺ｯ譛ｪ菴ｿ逕ｨ)   // -- 2020.12.15
    CONSTANT_BUFFER_WVLED()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_WVLED));
    }
};

// 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ逕ｨ縺ｮ蜷・ｨｮ繝・・繧ｿ繧呈ｸ｡縺吶◆繧√・讒矩菴・ // -- 2020.1.24
struct CONSTANT_BUFFER_DISPLACE
{
    VECTOR3 vEyePosInv; // 繝｢繝・Ν蠎ｧ讓咏ｳｻ縺九ｉ隕九◆隕也せ菴咲ｽｮ
    float fMinDistance; // 繝・ャ繧ｻ繝ｬ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譛蟆剰ｷ晞屬
    float fMaxDistance; // 繝・ャ繧ｻ繝ｬ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譛螟ｧ霍晞屬
    int iMaxDevide; // 譛螟ｧ蛻・牡謨ｰ
    VECTOR2 vHeight; // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ縺ｮ鬮倥＆菫よ焚
    VECTOR4 vWaveMove; // 豕｢縺ｮ遘ｻ蜍暮㍼・域ｳ｢繧ｷ繧ｧ繝ｼ繝繝ｼ逕ｨ・・
    VECTOR4 vSpecular; // 繧ｹ繝壹く繝･繝ｩ繝ｼ諠・ｱ・域ｳ｢繧ｷ繧ｧ繝ｼ繝繝ｼ逕ｨ・・
    CONSTANT_BUFFER_DISPLACE()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_DISPLACE));
    }
};

// 繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ螳壽焚繝舌ャ繝輔ぃ讒矩菴・  //  2017.8.25
struct CONSTANT_BUFFER_EFFECT
{
    MATRIX4X4 mWVP; // 繝ｯ繝ｼ繝ｫ繝峨・繝薙Η繝ｼ繝ｻ謚募ｽｱ螟画鋤陦悟・
    MATRIX4X4 mW; // 繝ｯ繝ｼ繝ｫ繝芽｡悟・
    MATRIX4X4 mV; // 繝薙Η繝ｼ陦悟・
    MATRIX4X4 mP; // 謚募ｽｱ陦悟・
    VECTOR2 vUVOffset; // 繝・け繧ｹ繝√Ε蠎ｧ讓吶・繧ｪ繝輔そ繝・ヨ
    VECTOR2 vUVScale; // 繝・け繧ｹ繝√Ε蠎ｧ讓吶・繧ｹ繧ｱ繝ｼ繝ｫ  // -- 2019.7.17
    float fAlpha; // 騾乗・蠎ｦ
    float fSize; // 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ螟ｧ縺阪＆  // -- 2018.8.23
    VECTOR2 Dummy; // 繝代ョ繧｣繝ｳ繧ｰ逕ｨ繝繝溘・繝吶け繝医Ν       // -- 2019.7.17
    CONSTANT_BUFFER_EFFECT()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_EFFECT));
    }
};


// 3D繧ｹ繝励Λ繧､繝医す繧ｧ繝ｼ繝繝ｼ逕ｨ縺ｮ螳壽焚繝舌ャ繝輔ぃ讒矩菴・
struct CONSTANT_BUFFER_SPRITE
{
    MATRIX4X4 mWVP;
    MATRIX4X4 mW;
    float ViewPortWidth;
    float ViewPortHeight;
    VECTOR2 vUVOffset;
    VECTOR4 vColor; // 繧ｫ繝ｩ繝ｼ・磯乗・蠎ｦ繧貞性繧・・
    VECTOR4 vMatInfo; // 繝槭ユ繝ｪ繧｢繝ｫ髢｢騾｣諠・ｱ・・:繝・け繧ｹ繝√Ε譛臥┌縺ｪ縺ｩ・峨・rawRect()縲．rawLine()縺ｧ菴ｿ逕ｨ縲・
    CONSTANT_BUFFER_SPRITE()
    {
        ZeroMemory(this, sizeof(CONSTANT_BUFFER_SPRITE));
    }
};

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


// 繝槭ユ繝ｪ繧｢繝ｫ諠・ｱ讒矩菴・                     // -- 2020.12.15
struct CONSTANT_BUFFER_MATERIAL
{
    VECTOR4 vMatDuffuse;
    VECTOR4 vMatSpecular;
};

//
// CShader繧ｯ繝ｩ繧ｹ
//
class CShader
{
public:
    // Direct3D11
    CDirect3D* m_pD3D;

    // 繧ｷ繧ｧ繝ｼ繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝・
    // 騾壼ｸｸ逕ｨ縺ｮ繧ｷ繝ｳ繝励Ν縺ｪ繧ｷ繧ｧ繝ｼ繝繝ｼ
    ID3D11InputLayout* m_pSimple_VertexLayout;
    ID3D11VertexShader* m_pSimple_VS;
    ID3D11PixelShader* m_pSimple_PS;

    // 3D繧ｹ繝励Λ繧､繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    ID3D11InputLayout* m_pSprite3D_VertexLayout;
    ID3D11VertexShader* m_pSprite3D_VS;
    ID3D11PixelShader* m_pSprite3D_PS;
    ID3D11VertexShader* m_pSprite3D_VS_BILL;

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ(豕｢)逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    ID3D11InputLayout* m_pDisplaceWave_VertexLayout;
    ID3D11VertexShader* m_pDisplaceWave_VS;
    ID3D11HullShader* m_pDisplaceWave_HS;
    ID3D11DomainShader* m_pDisplaceWave_DS;
    ID3D11PixelShader* m_pDisplaceWave_PS;

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ(繧ｹ繧ｭ繝ｳ繝｡繝・す繝･)逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    ID3D11VertexShader* m_pDisplaceSkinMesh_VS;
    ID3D11HullShader* m_pDisplaceSkinMesh_HS;
    ID3D11DomainShader* m_pDisplaceSkinMesh_DS;
    ID3D11PixelShader* m_pDisplaceSkinMesh_PS;

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ(繧ｹ繧ｿ繝・ぅ繝・け繝｡繝・す繝･)逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    ID3D11VertexShader* m_pDisplaceStaticMesh_VS;
    ID3D11HullShader* m_pDisplaceStaticMesh_HS;
    ID3D11DomainShader* m_pDisplaceStaticMesh_DS;
    ID3D11PixelShader* m_pDisplaceStaticMesh_PS;

    // 繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    ID3D11InputLayout* m_pEffect3D_VertexLayout;
    ID3D11VertexShader* m_pEffect3D_VS_POINT;
    ID3D11GeometryShader* m_pEffect3D_GS_POINT;
    ID3D11PixelShader* m_pEffect3D_PS;
    ID3D11InputLayout* m_pEffect3D_VertexLayout_BILL;
    ID3D11VertexShader* m_pEffect3D_VS_BILL;
    ID3D11VertexShader* m_pEffect3D_VS_BILLMESH; // -- 2019.7.17

    // Fbx繝｢繝・Ν・壹せ繧ｿ繝・ぅ繝・け繝｡繝・す繝･逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    // (Normal繝槭ャ繝斐Φ繧ｰ蟇ｾ蠢・
    ID3D11InputLayout* m_pFbxStaticMesh_VertexLayout;
    ID3D11VertexShader* m_pFbxStaticMesh_VS;
    ID3D11PixelShader* m_pFbxStaticMesh_PS;

    // Fbx繝｢繝・Ν・壹せ繧ｭ繝ｳ繝｡繝・す繝･逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    // (Normal繝槭ャ繝斐Φ繧ｰ蟇ｾ蠢・
    ID3D11InputLayout* m_pFbxSkinMesh_VertexLayout;
    ID3D11VertexShader* m_pFbxSkinMesh_VS;
    ID3D11PixelShader* m_pFbxSkinMesh_PS;
    ID3D11Buffer* m_pConstantBufferArc; 


    // 螳壽焚繝舌ャ繝輔ぃ  ------------------------------------------

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ逕ｨ縺ｮ螳壽焚繝舌ャ繝輔ぃ   // -- 2020.1.24
    ID3D11Buffer* m_pConstantBufferDisplace;

    // 繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ螳壽焚繝舌ャ繝輔ぃ
    ID3D11Buffer* m_pConstantBufferEffect;

    // 3D繧ｹ繝励Λ繧､繝育畑縺ｮ螳壽焚繝舌ャ繝輔ぃ
    ID3D11Buffer* m_pConstantBufferSprite3D;

    // 繝｡繝・す繝･縺ｮ螟画鋤陦悟・繝ｻ繧ｫ繝ｩ繝ｼ諠・ｱ逕ｨ螳壽焚繝舌ャ繝輔ぃ
    ID3D11Buffer* m_pConstantBufferWVLED; // -- 2020.1.24

    // 繝懊・繝ｳ陦悟・霆｢騾∫畑縺ｮ螳壽焚繝舌ャ繝輔ぃ
    ID3D11Buffer* m_pConstantBufferBone2;

    // 繝槭ユ繝ｪ繧｢繝ｫ諠・ｱ逕ｨ螳壽焚繝舌ャ繝輔ぃ
    ID3D11Buffer* m_pConstantBufferMaterial; // -- 2020.12.15


public:
    HRESULT InitShader();
    HRESULT InitShaderSimple();
    HRESULT InitShaderSprite();

    HRESULT InitShaderFbx();
    HRESULT InitShaderDisplace();
    HRESULT InitShaderEffect();
    HRESULT InitShaderConstant();

    HRESULT MakeShader(const TCHAR ProfileName[], const TCHAR FileName[], void** ppShader,
                       D3D11_INPUT_ELEMENT_DESC Fluid_layout[] = nullptr, UINT numElements = 0,
                       ID3D11InputLayout** ppInputLayout = nullptr);
    HRESULT MakeConstantBuffer(UINT size, ID3D11Buffer** ppConstantBuffer);

    CShader(CDirect3D* pD3D);
    ~CShader();
};
