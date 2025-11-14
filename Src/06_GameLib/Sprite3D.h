// ========================================================================================
//
// �R�c�p�̃X�v���C�g�̏���                                        ver 3.3        2024.10.5
//
//   �|���S���̕\�ʔ����ύX�B������\�Ƃ���
//   Sprite3D.cpp Direct3D.h ���ύX�ƂȂ��Ă���
//
//                                                                             Sprite3D.h
// ========================================================================================

#pragma once

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>

#include "../02_DirectX/Direct3D.h"
#include "../04_FrameWork/Shader.h"

//
// �}�N����`
//
// �쐬����t�H���g�e�N�X�`���̍ő吔
#define  TEXT_DATA_MAX  100

//
// �X�v���C�g�p���_�̍\����
//
struct SpriteVertex
{
	VECTOR3 Pos;   //�ʒu
	VECTOR2 UV;	   //�e�N�X�`���[���W
};

//
// CSpriteImage�N���X
//
class CShader;

class CSpriteImage
{
public:
	// Direct3D11
	CDirect3D*     m_pD3D;
	CShader*       m_pShader;

	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
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
// CSprite�N���X
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
	DWORD			m_dwDestWidth;    // 2D�X�v���C�g�̕\���傫��
	DWORD			m_dwDestHeight;   // 2D�X�v���C�g�̕\���傫

	float			m_fDestWidth;     // 3D�X�v���C�g�̕\���傫��(���[�g���P��)
	float			m_fDestHeight;    // 3D�X�v���C�g�̕\���傫��(���[�g���P��)

	ID3D11Buffer*   m_pVertexBufferSprite;
	ID3D11Buffer*   m_pVertexBufferLine;    // -- 2017.10.9
	ID3D11Buffer*   m_pVertexBufferRect;    // -- 2018.3.20
	ID3D11Buffer*	m_pVertexBufferBillSprite;   // -- 2018.8.10

public:
	CSpriteImage*   m_pImage;     // �X�v���C�g�C���[�W
	DWORD			m_ofX;        // �p�^�[���ʒu�I�t�Z�b�gX
	DWORD			m_ofY;        // �p�^�[���ʒu�I�t�Z�b�gY
	VECTOR4         m_vDiffuse;   // �f�B�t���[�Y�F(x:r y:g z:b w:a)�B�X�v���C�g�̂ݎg�p�B���C����l�p�`�ɂ͎g�p���Ȃ��B  // -- 2020.1.24
	int             m_nBlend;     // �u�����h�X�e�[�g  0:�ʏ� 1:���ߐF�i�����l�j 2:���Z����     // -- 2019.8.18

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
	bool Draw3DWithWorldMatrix(CSpriteImage* pImage, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha = 1.0f);

	void    SetShader();
	void    ResetShader();


};
// ========================================================================================
//
// �t�H���g�@�e�N�X�`���[�̏���
// 
// ========================================================================================
//
// �����t�H���g�̍\����
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
// �e�L�X�g�f�[�^�̍\����
//
struct TextData
{
	DWORD                       m_dwKbn;         // 0:2D�t�H���g 1:3D�t�H���g   // -- 2018.8.10
	TCHAR*                      m_szText;
	int                         m_iFontsize;
	DWORD                       m_dwColor;
	float                       m_fAlpha;
	ID3D11ShaderResourceView*	m_pResourceView; //�e�N�X�`���[���\�[�X�r���[
	ID3D11Buffer*				m_pVertexBufferFont;
	TextData()
	{
		ZeroMemory(this, sizeof(TextData));
	}
};
// ========================================================================================
//
// CFontTexture�N���X
//
// ========================================================================================
class CFontTexture
{
protected:
	// Direct3D11
	CDirect3D*		m_pD3D;
	CShader*        m_pShader;

	TextData		m_TextData[TEXT_DATA_MAX];
	DWORD			m_Idx;       // TextData�̃C���f�b�N�X

	// �R�c�t�H���g   // -- 2018.8.10
	float			m_fDestWidth;     // 3D�X�v���C�g�̕\���傫��(���[�g���P��)
	float			m_fDestHeight;    // 3D�X�v���C�g�̕\���傫��(���[�g���P��)

public:
	void Refresh();
	void Draw(float posX, float posY, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
	void Draw(MATRIX4X4 mWorld, const TCHAR* text, int fontsize, DWORD colorABGR, float fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
	void SetShader();
	void ResetShader();
	void CreateVB(const DWORD& dwWidth, const DWORD& dwHeight);
	void CreateTex(const DWORD& dwKbn, const float&  fDestWidth, const float&  fDestHeight, const TCHAR* text, const int& fontsize, const DWORD& colorABGR, const TCHAR* fontname);

	// �R�c�t�H���g
	HRESULT CreateVB3D(const float& fDestWidth, const float& fDestHeight);
	bool Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha = 1.0f, const TCHAR* szFontName = nullptr);
	bool Draw3D(const VECTOR3& vPos, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha = 1.0f, const TCHAR* szFontName = nullptr);		// -- 2024.3.23

	CFontTexture(CShader* pShader);
	~CFontTexture();

};





