// ========================================================================================
//
// 3Dおよび2Dスプライト描画ライブラリ                                ver 3.3        2024.10.5
//
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
    VECTOR3 Pos; // 位置
    VECTOR2 UV; // テクスチャ座標
};

/// <summary>汎用ユーティリティで使う Circle Draw Params の情報と処理をまとめる型</summary>
struct CircleDrawParams
{
    float startAngle;
    float endAngle;
    VECTOR3 center;
};

/// <summary>汎用ユーティリティで使う Arc Draw Params の情報と処理をまとめる型</summary>
struct ArcDrawParams
{
    float startAngle = 0.0f; // 開始角度（度、0=12時）Degrees
    float ratio = 1.0f; // 塗り割合（0〜1）
    float innerRadius = 0.0f; // 内半径（0〜0.5）
    bool clockwise = true;
};

//
// CSpriteImageクラス
// (テクスチャリソースを管理するクラス)
//
class CShader;

/// <summary>汎用ユーティリティで使う Sprite Image の情報と処理をまとめる型</summary>
class CSpriteImage
{
public:
    // Direct3D11
    CDirect3D* m_pD3D;
    CShader* m_pShader;

    ID3D11ShaderResourceView* m_pTexture; // テクスチャ（シェーダーリソースビュー）
    DWORD m_dwImageWidth;
    DWORD m_dwImageHeight;

    /// CSpriteImage を初期化する
    CSpriteImage();
    /// CSpriteImage を初期化する
    /// @param TName 名前
    CSpriteImage(const TCHAR* TName);
    /// CSpriteImage を初期化する
    /// @param pShader pShader に渡す値
    CSpriteImage(CShader* pShader);
    /// CSpriteImage を初期化する
    /// @param pShader pShader に渡す値
    /// @param TName 名前
    CSpriteImage(CShader* pShader, const TCHAR* TName);
    /// CSpriteImage の終了処理を行う
    ~CSpriteImage();
    /// 読み込む
    /// @param TName 名前
    /// @return 処理結果
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

    DWORD m_dwSrcX;
    DWORD m_dwSrcY;
    DWORD m_dwSrcWidth;
    DWORD m_dwSrcHeight;
    DWORD m_dwDestWidth; // 2Dスプライトの表示幅
    DWORD m_dwDestHeight; // 2Dスプライトの表示高さ

    float m_fDestWidth; // 3Dスプライトの表示幅(ワールド単位)
    float m_fDestHeight; // 3Dスプライトの表示高さ(ワールド単位)

    ID3D11Buffer* m_pVertexBufferSprite;
    ID3D11Buffer* m_pVertexBufferLine; // ライン用頂点バッファ
    ID3D11Buffer* m_pVertexBufferRect; // 矩形用頂点バッファ
    ID3D11Buffer* m_pVertexBufferBillSprite; // ビルボード用頂点バッファ

public:
    CSpriteImage* m_pImage; // 使用するスプライトイメージ
    DWORD m_ofX; // パターン位置オフセットX
    DWORD m_ofY; // パターン位置オフセットY
    VECTOR4 m_vDiffuse; // ディフューズ色(RGBA) ※スプライトのみ使用
    int m_nBlend; // ブレンドステート 0:通常 1:半透明 2:加算合成

public:
    /// CSprite を初期化する
    CSprite();
    /// CSprite を初期化する
    /// @param pShader pShader に渡す値
    CSprite(CShader* pShader);
    /// CSprite を初期化する
    /// @param pImage pImage に渡す値
    CSprite(CSpriteImage* pImage);
    /// CSprite を初期化する
    /// @param pImage pImage に渡す値
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight);
    /// CSprite を初期化する
    /// @param pImage pImage に渡す値
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    /// @param destwidth 幅
    /// @param destheight 高さ
    CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight,
            const DWORD& destwidth, const DWORD& destheight);
    /// CSprite の終了処理を行う
    ~CSprite();
    /// Image を設定する
    /// @param pImage pImage に渡す値
    void SetImage(CSpriteImage* pImage);
    /// Src を設定する
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    /// @return 処理結果
    HRESULT SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight);
    /// Src を設定する
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    /// @param destwidth 幅
    /// @param destheight 高さ
    /// @return 処理結果
    HRESULT SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight,
                   const DWORD& destwidth, const DWORD& destheight);
    /// Src を設定する
    /// @param pImage pImage に渡す値
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param width 幅
    /// @param height 高さ
    /// @return 処理結果
    HRESULT SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& width, const DWORD& height);
    /// Src を設定する
    /// @param pImage pImage に渡す値
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param width 幅
    /// @param height 高さ
    /// @param destwidth 幅
    /// @param destheight 高さ
    /// @return 処理結果
    HRESULT SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& width, const DWORD& height,
                   const DWORD& destwidth, const DWORD& destheight);
    /// Src X を取得する
    /// @return 処理結果
    DWORD GetSrcX() { return m_dwSrcX; }
    /// Src Y を取得する
    /// @return 処理結果
    DWORD GetSrcY() { return m_dwSrcY; }
    /// Src Width を取得する
    /// @return 処理結果
    DWORD GetSrcWidth() { return m_dwSrcWidth; }
    /// Src Height を取得する
    /// @return 処理結果
    DWORD GetSrcHeight() { return m_dwSrcHeight; }
    /// Dest Width を取得する
    /// @return 処理結果
    DWORD GetDestWidth() { return m_dwDestWidth; }
    /// Dest Height を取得する
    /// @return 処理結果
    DWORD GetDestHeight() { return m_dwDestHeight; }
    /// FDest Width を取得する
    /// @return 計算結果の値
    float GetFDestWidth() { return m_fDestWidth; }
    /// FDest Height を取得する
    /// @return 計算結果の値
    float GetFDestHeight() { return m_fDestHeight; }

    // 2D描画
    /// 描画する
    /// @param posX 座標
    /// @param posY 座標
    void Draw(const float& posX, const float& posY);
    /// 描画する
    /// @param mWorld mWorld に渡す値
    void Draw(const MATRIX4X4& mWorld);
    /// 描画する
    /// @param pImage pImage に渡す値
    /// @param posX 座標
    /// @param posY 座標
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    /// @param fAlpha 透明度
    void Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY,
              const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha = 1.0f);
    /// 描画する
    /// @param pImage pImage に渡す値
    /// @param posX 座標
    /// @param posY 座標
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    /// @param destwidth 幅
    /// @param destheight 高さ
    /// @param fAlpha 透明度
    void Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY,
              const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight,
              const float& fAlpha = 1.0f);
    /// 描画する
    /// @param pImage pImage に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    /// @param fAlpha 透明度
    void Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY,
              const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha = 1.0f);
    /// 描画する
    /// @param pImage pImage に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcwidth 幅
    /// @param srcheight 高さ
    /// @param destwidth 幅
    /// @param destheight 高さ
    /// @param fAlpha 透明度
    void Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY,
              const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight,
              const float& fAlpha = 1.0f);
    /// Line を描画する
    /// @param StartX StartX に渡す値
    /// @param StartY StartY に渡す値
    /// @param EndX EndX に渡す値
    /// @param EndY EndY に渡す値
    /// @param WidthIn 幅
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    void DrawLine(const float& StartX, const float& StartY, const float& EndX, const float& EndY, const DWORD& WidthIn,
                  const DWORD& colorABGR, const float& fAlpha = 1.0f);
    /// Rect を描画する
    /// @param posX 座標
    /// @param posY 座標
    /// @param width 幅
    /// @param height 高さ
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    void DrawRect(const float& posX, const float& posY, const DWORD& width, const DWORD& height, const DWORD& colorABGR,
                  const float& fAlpha = 1.0f);
    /// Rect を描画する
    /// @param mWorld mWorld に渡す値
    /// @param width 幅
    /// @param height 高さ
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    void DrawRect(const MATRIX4X4& mWorld, const DWORD& width, const DWORD& height, const DWORD& colorABGR,
                  const float& fAlpha = 1.0f);
    // 自作関数: 円形描画
    /// Circle を描画する
    /// @param pImage pImage に渡す値
    /// @param posX 座標
    /// @param posY 座標
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcWid srcWid に渡す値
    /// @param srcHei srcHei に渡す値
    /// @param startRad startRad に渡す値
    /// @param endRad endRad に渡す値
    /// @param fAlpha 透明度
    void DrawCircle(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcWid, DWORD srcHei,
                    float startRad = 0, float endRad = 6.28318f, float fAlpha = 1.0f);
    /// Arc を描画する
    /// @param pImage pImage に渡す値
    /// @param posX 座標
    /// @param posY 座標
    /// @param srcX srcX に渡す値
    /// @param srcY srcY に渡す値
    /// @param srcWid srcWid に渡す値
    /// @param srcHei srcHei に渡す値
    /// @param arcParams arcParams に渡す値
    /// @param fAlpha 透明度
    void DrawArc(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcWid,
                 DWORD srcHei, ArcDrawParams& arcParams, float fAlpha);
    /// Src3 D を設定する
    /// @param fDestWidth 幅
    /// @param fDestHeight 高さ
    /// @param dwSrcX dwSrcX に渡す値
    /// @param dwSrcY dwSrcY に渡す値
    /// @param dwSrcWidth 幅
    /// @param dwSrcHeight 高さ
    /// @return 処理結果
    HRESULT SetSrc3D(const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX, const DWORD& dwSrcY,
                     const DWORD& dwSrcWidth, const DWORD& dwSrcHeight);
    /// Src3 D を設定する
    /// @param pImage pImage に渡す値
    /// @param fDestWidth 幅
    /// @param fDestHeight 高さ
    /// @param dwSrcX dwSrcX に渡す値
    /// @param dwSrcY dwSrcY に渡す値
    /// @param dwSrcWidth 幅
    /// @param dwSrcHeight 高さ
    /// @return 処理結果
    HRESULT SetSrc3D(CSpriteImage* pImage, const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX,
                     const DWORD& dwSrcY, const DWORD& dwSrcWidth, const DWORD& dwSrcHeight);

    /// 3 D を描画する
    /// @param pImage pImage に渡す値
    /// @param vPos 座標
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vEye vEye に渡す値
    /// @param vSize サイズ
    /// @param vSrcPos 座標
    /// @param vSrcSize サイズ
    /// @param fAlpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                const VECTOR3& vEye, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize,
                const float& fAlpha = 1.0f);
    /// 3 D を描画する
    /// @param vPos 座標
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vEye vEye に渡す値
    /// @param vSize サイズ
    /// @param vSrcPos 座標
    /// @param vSrcSize サイズ
    /// @param fAlpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye,
                const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    /// 3 D を描画する
    /// @param vPos 座標
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vEye vEye に渡す値
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye);
    /// Line3 D を描画する
    /// @param vStart vStart に渡す値
    /// @param vEnd vEnd に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vEye vEye に渡す値
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                    const VECTOR3& vEye, const DWORD& colorABGR, const float& fAlpha = 1.0f);

    /// 3 D を描画する
    /// @param pImage pImage に渡す値
    /// @param vPos 座標
    /// @param vSize サイズ
    /// @param vSrcPos 座標
    /// @param vSrcSize サイズ
    /// @param fAlpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos,
                const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    /// 3 D を描画する
    /// @param vPos 座標
    /// @param vSize サイズ
    /// @param vSrcPos 座標
    /// @param vSrcSize サイズ
    /// @param fAlpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize,
                const float& fAlpha = 1.0f);
    /// 3 D を描画する
    /// @param vPos 座標
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(const VECTOR3& vPos);
    /// Line3 D を描画する
    /// @param vStart vStart に渡す値
    /// @param vEnd vEnd に渡す値
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const DWORD& colorABGR, const float& fAlpha = 1.0f);
    /// 3 DWith World Matrix を描画する
    /// @param pImage pImage に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vSize サイズ
    /// @param vSrcPos 座標
    /// @param vSrcSize サイズ
    /// @param fAlpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool Draw3DWithWorldMatrix(CSpriteImage* pImage, const MATRIX4X4& mWorld, const MATRIX4X4& mView,
                               const MATRIX4X4& mProj, const VECTOR2& vSize, const VECTOR2& vSrcPos,
                               const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
    /// World を描画する
    /// @param pImage pImage に渡す値
    /// @param vPos 座標
    /// @param radius 半径
    /// @param alpha 透明度
    /// @return 成功または条件を満たす場合 true
    bool DrawWorld(CSpriteImage* pImage, const VECTOR3& vPos, float radius, float alpha = 1.0f);

    /// Shader を設定する
    void SetShader();
    /// Shader を初期状態に戻す
    void ResetShader();
};

// ========================================================================================
//
// 
// ========================================================================================

//
// フォントデータの構造体
//
struct FontData
{
    TEXTMETRIC TM;
    GLYPHMETRICS GM;
    BYTE* ptr;
    /// FontData を初期化する
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
    DWORD m_dwKbn; // 0:2Dフォント 1:3Dフォント
    TCHAR* m_szText;
    int m_iFontsize;
    DWORD m_dwColor;
    float m_fAlpha;
    ID3D11ShaderResourceView* m_pResourceView; // テクスチャ（シェーダーリソースビュー）
    ID3D11Buffer* m_pVertexBufferFont;
    /// TextData を初期化する
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

    TextData m_TextData[TEXT_DATA_MAX];
    DWORD m_Idx; // TextDataのインデックス

    // 3Dフォント用設定
    float m_fDestWidth; // 3D表示時の幅(ワールド単位)
    float m_fDestHeight; // 3D表示時の高さ(ワールド単位)

public:
    /// Refresh の処理を行う
    void Refresh();
    /// 描画する
    /// @param posX 座標
    /// @param posY 座標
    /// @param text 使用するテクスチャ
    /// @param fontsize サイズ
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    /// @param szFontName 名前
    void Draw(float posX, float posY, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f,
              const TCHAR* szFontName = nullptr);
    /// 描画する
    /// @param mWorld mWorld に渡す値
    /// @param text 使用するテクスチャ
    /// @param fontsize サイズ
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    /// @param szFontName 名前
    void Draw(MATRIX4X4 mWorld, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f,
              const TCHAR* szFontName = nullptr);
    /// Shader を設定する
    void SetShader();
    /// Shader を初期状態に戻す
    void ResetShader();
    /// VB を作成する
    /// @param dwWidth 幅
    /// @param dwHeight 高さ
    void CreateVB(const DWORD& dwWidth, const DWORD& dwHeight);
    /// Tex を作成する
    /// @param dwKbn dwKbn に渡す値
    /// @param fDestWidth 幅
    /// @param fDestHeight 高さ
    /// @param text 使用するテクスチャ
    /// @param fontsize サイズ
    /// @param colorABGR ABGR形式の色
    /// @param fontname 名前
    void CreateTex(const DWORD& dwKbn, const float& fDestWidth, const float& fDestHeight, const TCHAR* text,
                   const int& fontsize, const DWORD& colorABGR, const TCHAR* fontname);

    // 3Dフォント（ビルボード）
    /// VB3 D を作成する
    /// @param fDestWidth 幅
    /// @param fDestHeight 高さ
    /// @return 処理結果
    HRESULT CreateVB3D(const float& fDestWidth, const float& fDestHeight);
    /// 3 D を描画する
    /// @param vPos 座標
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vEye vEye に渡す値
    /// @param szText 使用するテクスチャ
    /// @param vSize サイズ
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    /// @param szFontName 名前
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye,
                const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha = 1.0f,
                const TCHAR* szFontName = nullptr);
    /// 3 D を描画する
    /// @param vPos 座標
    /// @param szText 使用するテクスチャ
    /// @param vSize サイズ
    /// @param colorABGR ABGR形式の色
    /// @param fAlpha 透明度
    /// @param szFontName 名前
    /// @return 成功または条件を満たす場合 true
    bool Draw3D(const VECTOR3& vPos, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR,
                const float& fAlpha = 1.0f, const TCHAR* szFontName = nullptr);

    /// CFontTexture を初期化する
    /// @param pShader pShader に渡す値
    CFontTexture(CShader* pShader);
    /// CFontTexture の終了処理を行う
    ~CFontTexture();
};
