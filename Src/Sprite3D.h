// ========================================================================================
//
// ３Ｄ用のスプライトの処理                                        ver 3.3        2024.10.5
//
//   ポリゴンの表面判定を変更。左回りを表とする
//   Sprite3D.cpp Direct3D.h が変更となっている
//
//                                                                             Sprite3D.h
// ========================================================================================

#pragma once

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Direct3D.h"
#include "Shader.h"

//
// マクロ定義
//
// 作成するフォントテクスチャの最大数
#define  TEXT_DATA_MAX  100

//
// スプライト用頂点の構造体
//
struct SpriteVertex
{
	VECTOR3 Pos;   //位置
	VECTOR2 UV;	   //テクスチャー座標
};

//
// CSpriteImageクラス
//
class CShader;

class CSpriteImage
{
public:
	// Direct3D11
	CDirect3D*     m_pD3D;
	CShader*       m_pShader;

	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）
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
//
class CSprite
{
protected:
	// Direct3D11
	CDirect3D*      m_pD3D;
	CShader*        m_pShader;

	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;
	DWORD			m_dwDestWidth;    // 2Dスプライトの表示大きさ
	DWORD			m_dwDestHeight;   // 2Dスプライトの表示大き

	float			m_fDestWidth;     // 3Dスプライトの表示大きさ(メートル単位)
	float			m_fDestHeight;    // 3Dスプライトの表示大きさ(メートル単位)

	ID3D11Buffer*   m_pVertexBufferSprite;
	ID3D11Buffer*   m_pVertexBufferLine;    // -- 2017.10.9
	ID3D11Buffer*   m_pVertexBufferRect;    // -- 2018.3.20
	ID3D11Buffer*	m_pVertexBufferBillSprite;   // -- 2018.8.10

public:
	CSpriteImage*   m_pImage;     // スプライトイメージ
	DWORD			m_ofX;        // パターン位置オフセットX
	DWORD			m_ofY;        // パターン位置オフセットY
	VECTOR4         m_vDiffuse;   // ディフューズ色(x:r y:g z:b w:a)。スプライトのみ使用。ラインや四角形には使用しない。  // -- 2020.1.24
	int             m_nBlend;     // ブレンドステート  0:通常 1:透過色（初期値） 2:加算合成     // -- 2019.8.18

public:
	CSprite();
	CSprite(CShader* pShader);
	CSprite(CSpriteImage* pImage);    // -- 2017.10.9
	CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight);
	CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight);
	~CSprite();
	void    SetImage(CSpriteImage* pImage);    // -- 2017.10.9
	HRESULT SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight);
	HRESULT SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight);
	HRESULT SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& width, const DWORD& height);
	HRESULT SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& width, const DWORD& height, const DWORD& destwidth, const DWORD& destheight);
	DWORD   GetSrcX() { return m_dwSrcX; }    // -- 2017.10.9
	DWORD   GetSrcY() { return m_dwSrcY; }    // -- 2017.10.9
	DWORD   GetSrcWidth() { return m_dwSrcWidth; }
	DWORD   GetSrcHeight() { return m_dwSrcHeight; }
	DWORD   GetDestWidth() { return m_dwDestWidth; }
	DWORD   GetDestHeight() { return m_dwDestHeight; }
	float   GetFDestWidth() { return m_fDestWidth; }      // 2018.9.30
	float   GetFDestHeight() { return m_fDestHeight; }    // 2018.9.30

	void    Draw(const float& posX, const float& posY);
	void    Draw(const MATRIX4X4& mWorld);
	void    Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha = 1.0f);
	void    Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight, const float& fAlpha = 1.0f);
	void    Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha = 1.0f);
	void    Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight, const float& fAlpha = 1.0f);
	void    DrawLine(const float& StartX, const float& StartY, const float& EndX, const float& EndY, const DWORD& WidthIn, const DWORD& colorABGR, const float& fAlpha = 1.0f);
	void    DrawRect(const float& posX, const float& posY, const DWORD& width, const DWORD& height, const DWORD& colorABGR, const float& fAlpha = 1.0f);    // -- 2018.3.20

	// 3DSprite   // -- 2018.8.10
	HRESULT SetSrc3D(const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX, const DWORD& dwSrcY, const DWORD& dwSrcWidth, const DWORD& dwSrcHeight);   // 2018.9.30
	HRESULT SetSrc3D(CSpriteImage* pImage, const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX, const DWORD& dwSrcY, const DWORD& dwSrcWidth, const DWORD& dwSrcHeight);

	bool Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
	bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);
	bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye);
	bool DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const DWORD& colorABGR, const float& fAlpha = 1.0f);  // -- 2020.1.24

	bool Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);   // -- 2024.3.23
	bool Draw3D(const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);    // -- 2024.3.23
	bool Draw3D(const VECTOR3& vPos);																					// -- 2024.3.23
	bool DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const DWORD& colorABGR, const float& fAlpha = 1.0f);  // -- 2024.3.23


	void    SetShader();
	void    ResetShader();


};
// ========================================================================================
//
// フォント　テクスチャーの処理
// 
// ========================================================================================
//
// 文字フォントの構造体
//
struct FontData
{
	TEXTMETRIC   TM;
	GLYPHMETRICS GM;
	BYTE*        ptr;
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
	DWORD                       m_dwKbn;         // 0:2Dフォント 1:3Dフォント   // -- 2018.8.10
	TCHAR*                      m_szText;
	int                         m_iFontsize;
	DWORD                       m_dwColor;
	float                       m_fAlpha;
	ID3D11ShaderResourceView*	m_pResourceView; //テクスチャーリソースビュー
	ID3D11Buffer*				m_pVertexBufferFont;
	TextData()
	{
		ZeroMemory(this, sizeof(TextData));
	}
};
// ========================================================================================
//
// CFontTextureクラス
//
// ========================================================================================
class CFontTexture
{
protected:
	// Direct3D11
	CDirect3D*		m_pD3D;
	CShader*        m_pShader;

	TextData		m_TextData[TEXT_DATA_MAX];
	DWORD			m_Idx;       // TextDataのインデックス

	// ３Ｄフォント   // -- 2018.8.10
	float			m_fDestWidth;     // 3Dスプライトの表示大きさ(メートル単位)
	float			m_fDestHeight;    // 3Dスプライトの表示大きさ(メートル単位)

public:
	void Refresh();
	void Draw(float posX, float posY, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
	void Draw(MATRIX4X4 mWorld, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
	void SetShader();
	void ResetShader();
	void CreateVB(const DWORD& dwWidth, const DWORD& dwHeight);
	void CreateTex(const DWORD& dwKbn, const float&  fDestWidth, const float&  fDestHeight, const TCHAR* text, const int& fontsize, const DWORD& colorABGR, const TCHAR* fontname);

	// ３Ｄフォント
	HRESULT CreateVB3D(const float& fDestWidth, const float& fDestHeight);
	bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
	bool Draw3D(const VECTOR3& vPos, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha = 1.0f, const TCHAR* szFontName = nullptr);		// -- 2024.3.23

	CFontTexture(CShader* pShader);
	~CFontTexture();

};





