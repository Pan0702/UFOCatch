// ========================================================================================
//
// 3Dおよび2Dスプライト描画ライブラリ                                ver 3.3        2024.10.5
//
//   ポリゴンでの画像表示、およびライン・矩形表示を行います
//   Sprite3D.cpp Direct3D.h などと連携して動作します
//
//                                                                             Sprite3D.h
// ========================================================================================

#pragma once

// ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "../Core/Graphics/Direct3D.h"
#include "../Framework/Shader.h"

//
// マクロ定義
//
// 作成可能なフォントテクスチャの最大数
#define  TEXT_DATA_MAX  100

//
// スプライト用頂点の構造体
//
struct SpriteVertex
{
    VECTOR3 Pos;   // 位置
    VECTOR2 UV;    // テクスチャ座標
};

struct CircleDrawParams
{
    float startAngle;
    float endAngle;
    VECTOR3 center;
};

//
// CSpriteImageクラス
// (テクスチャリソースを管理するクラス)
//
class CShader;

class CSpriteImage
{
public:
    // Direct3D11
    CDirect3D* m_pD3D;
    CShader* m_pShader;

    ID3D11ShaderResourceView* m_pTexture; // テクスチャ（シェーダーリソースビュー）
    DWORD          m_dwImageWidth;
    DWORD          m_dwImageHeight;

    CSpriteImage();
    CSpriteImage(const TCHAR* TName);
    CSpriteImage(CShader* pShader);
    CSpriteImage(CShader* pShader, const TCHAR* TName);
    ~CSpriteImage();
    HRESULT Load(const TCHAR* TName);
};

//
// CSpriteクラス
// (実際に描画命令を出すクラス)
//
class CSprite
{
protected:
    // Direct3D11
    CDirect3D* m_pD3D;
    CShader* m_pShader;

    DWORD        m_dwSrcX;
    DWORD        m_dwSrcY;
    DWORD        m_dwSrcWidth;
    DWORD        m_dwSrcHeight;
    DWORD        m_dwDestWidth;    // 2Dスプライトの表示幅
    DWORD        m_dwDestHeight;   // 2Dスプライトの表示高さ

    float        m_fDestWidth;     // 3Dスプライトの表示幅(ワールド単位)
    float        m_fDestHeight;    // 3Dスプライトの表示高さ(ワールド単位)

    ID3D11Buffer* m_pVertexBufferSprite;
    ID3D11Buffer* m_pVertexBufferLine;    // ライン用頂点バッファ
    ID3D11Buffer* m_pVertexBufferRect;    // 矩形用頂点バッファ
    ID3D11Buffer* m_pVertexBufferBillSprite; // ビルボード用頂点バッファ

public:
    CSpriteImage* m_pImage;     // 使用するスプライトイメージ
    DWORD        m_ofX;        // パターン位置オフセットX
    DWORD        m_ofY;        // パターン位置オフセットY
    VECTOR4         m_vDiffuse;   // ディフューズ色(RGBA) ※スプライトのみ使用
    int             m_nBlend;     // ブレンドステート 0:通常 1:半透明 2:加算合成

public:
    CSprite();
    CSprite(CShader* pShader);
    CSprite(CSpriteImage* pImage);
    CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight);
    CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight);
    ~CSprite();
    void    SetImage(CSpriteImage* pImage);
    HRESULT SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight);
    HRESULT SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight);
    HRESULT SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& width, const DWORD& height);
    HRESULT SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& width, const DWORD& height, const DWORD& destwidth, const DWORD& destheight);
    DWORD   GetSrcX() { return m_dwSrcX; }
    DWORD   GetSrcY() { return m_dwSrcY; }
    DWORD   GetSrcWidth() { return m_dwSrcWidth; }
    DWORD   GetSrcHeight() { return m_dwSrcHeight; }
    DWORD   GetDestWidth() { return m_dwDestWidth; }
    DWORD   GetDestHeight() { return m_dwDestHeight; }
    float   GetFDestWidth() { return m_fDestWidth; }
    float   GetFDestHeight() { return m_fDestHeight; }

    // 2D描画
    void    Draw(const float& posX, const float& posY);
    void    Draw(const MATRIX4X4& mWorld);
    void    Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha = 1.0f);
    void    Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight, const float& fAlpha = 1.0f);
    void    Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha = 1.0f);
    void    Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight, const float& fAlpha = 1.0f);
    void    DrawLine(const float& StartX, const float& StartY, const float& EndX, const float& EndY, const DWORD& WidthIn, const DWORD& colorABGR, const float& fAlpha = 1.0f);
    void    DrawRect(const float& posX, const float& posY, const DWORD& width, const DWORD& height, const DWORD& colorABGR, const float& fAlpha = 1.0f);
    
    // 自作関数: 円形描画
    void   DrawCircle(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcWid, DWORD srcHei, float startRad = 0, float endRad = 6.28318f, float fAlpha = 1.0f);
    
    // 3D描画・ビルボード
    HRESULT SetSrc3D(const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX, const DWORD& dwSrcY, const DWORD& dwSrcWidth, const DWORD& dwSrcHeight);
    HRESULT SetSrc3D(CSpriteImage* pImage, const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX, const DWORD& dwSrcY, const DWORD& dwSrcWidth, const DWORD& dwSrcHeight);

    bool Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye);
    bool DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const DWORD& colorABGR, const float& fAlpha = 1.0f);

    bool Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    bool Draw3D(const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    bool Draw3D(const VECTOR3& vPos);
    bool DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const DWORD& colorABGR, const float& fAlpha = 1.0f);
    bool Draw3DWithWorldMatrix(CSpriteImage* pImage, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    bool DrawWorld(CSpriteImage* pImage, const VECTOR3& vPos, float radius, float alpha = 1.0f);

    void    SetShader();
    void    ResetShader();
};

// ========================================================================================
//
// フォント・テキスト処理
// 
// ========================================================================================

//
// フォントデータの構造体
//
struct FontData
{
    TEXTMETRIC   TM;
    GLYPHMETRICS GM;
    BYTE* ptr;
    FontData()
    {
       ZeroMemory(this, sizeof(FontData));
    }
};

//
// テキストデータの構造体
//
struct TextData
{
    DWORD                       m_dwKbn;         // 0:2Dフォント 1:3Dフォント
    TCHAR* m_szText;
    int                         m_iFontsize;
    DWORD                       m_dwColor;
    float                       m_fAlpha;
    ID3D11ShaderResourceView* m_pResourceView; // テクスチャ（シェーダーリソースビュー）
    ID3D11Buffer* m_pVertexBufferFont;
    TextData()
    {
       ZeroMemory(this, sizeof(TextData));
    }
};

// ========================================================================================
//
// CFontTextureクラス
// (文字列を画像として描画するクラス)
//
// ========================================================================================
class CFontTexture
{
protected:
    // Direct3D11
    CDirect3D* m_pD3D;
    CShader* m_pShader;

    TextData      m_TextData[TEXT_DATA_MAX];
    DWORD        m_Idx;       // TextDataのインデックス

    // 3Dフォント用設定
    float        m_fDestWidth;     // 3D表示時の幅(ワールド単位)
    float        m_fDestHeight;    // 3D表示時の高さ(ワールド単位)

public:
    void Refresh();
    void Draw(float posX, float posY, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
    void Draw(MATRIX4X4 mWorld, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
    void SetShader();
    void ResetShader();
    void CreateVB(const DWORD& dwWidth, const DWORD& dwHeight);
    void CreateTex(const DWORD& dwKbn, const float& fDestWidth, const float& fDestHeight, const TCHAR* text, const int& fontsize, const DWORD& colorABGR, const TCHAR* fontname);

    // 3Dフォント（ビルボード）
    HRESULT CreateVB3D(const float& fDestWidth, const float& fDestHeight);
    bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
    bool Draw3D(const VECTOR3& vPos, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha = 1.0f, const TCHAR* szFontName = nullptr);

    CFontTexture(CShader* pShader);
    ~CFontTexture();
};
