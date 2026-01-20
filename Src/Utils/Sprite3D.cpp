// ========================================================================================
//
// �R�c�p�̃X�v���C�g�̏���                                        ver 3.3        2024.10.5
//
//   �|���S���̕\�ʔ����ύX�B�E����\�Ƃ���
//   Sprite3D.cpp Direct3D.h ���ύX�ƂȂ��Ă���
//
//                                                                             Sprite3D.cpp
// ========================================================================================

#include "Sprite3D.h"
#include "../Core/Game/GameMain.h"
#pragma warning(disable : 6387)

CSpriteImage::CSpriteImage() : CSpriteImage(GameDevice()->m_pShader)
{
}

CSpriteImage::CSpriteImage(const TCHAR* TName) : CSpriteImage(GameDevice()->m_pShader, TName)
{
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�C���[�W�̃R���X�g���N�^	
//
//  �����@CShader* pShader
//
//------------------------------------------------------------------------
CSpriteImage::CSpriteImage(CShader* pShader)
{
    ZeroMemory(this, sizeof(CSpriteImage));
    m_pShader = pShader;
    m_pD3D = pShader->m_pD3D;
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�C���[�W�̃R���X�g���N�^	
//
//	CShader* pShader
//  const TCHAR*   TName  �X�v���C�g�t�@�C����
//
//------------------------------------------------------------------------
CSpriteImage::CSpriteImage(CShader* pShader, const TCHAR* TName)
{
    ZeroMemory(this, sizeof(CSpriteImage));
    m_pShader = pShader;
    m_pD3D = pShader->m_pD3D;
    Load(TName);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�C���[�W�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CSpriteImage::~CSpriteImage()
{
    SAFE_RELEASE(m_pTexture);
}

//------------------------------------------------------------------------
//	�X�v���C�g�C���[�W�̓ǂݍ���	
//
//	�w�肵���X�v���C�g�t�@�C�����̃C���[�W��ǂݍ���
//
//  const TCHAR*   TName  �X�v���C�g�t�@�C����
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �t�@�C������������Ȃ�
//------------------------------------------------------------------------
HRESULT CSpriteImage::Load(const TCHAR* TName)
{
    if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(TName, &m_pTexture, m_dwImageWidth, m_dwImageHeight)))
    {
        MessageBox(0, _T("�X�v���C�g�@�e�N�X�`���[��ǂݍ��߂܂���"), TName, MB_OK);
        return E_FAIL;
    }
    return S_OK;
}

CSprite::CSprite() : CSprite(GameDevice()->m_pShader)
{
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃R���X�g���N�^	
//
//  �����@CShader* pShader
//
//------------------------------------------------------------------------
CSprite::CSprite(CShader* pShader)
{
    ZeroMemory(this, sizeof(CSprite));
    m_pShader = pShader;
    m_pD3D = pShader->m_pD3D;
    m_vDiffuse = VECTOR4(1, 1, 1, 1); // -- 2020.1.24
    m_nBlend = 1;
}

//------------------------------------------------------------------------
//                                                         // -- 2017.10.9
//	�X�v���C�g�̃R���X�g���N�^	
//
//  ����
//  CSpriteImage* pImage      �X�v���C�g�C���[�W�|�C���^
//
//------------------------------------------------------------------------
CSprite::CSprite(CSpriteImage* pImage)
{
    ZeroMemory(this, sizeof(CSprite));
    m_pShader = pImage->m_pShader;
    m_pD3D = pImage->m_pD3D;
    m_vDiffuse = VECTOR4(1, 1, 1, 1); // -- 2020.1.24
    m_nBlend = 1;
    m_pImage = pImage;
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃R���X�g���N�^	
//
//  CSpriteImage*        pImage      �X�v���C�g�C���[�W�|�C���^
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//
//------------------------------------------------------------------------
CSprite::CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth,
                 const DWORD& srcheight)
{
    ZeroMemory(this, sizeof(CSprite));
    m_pShader = pImage->m_pShader;
    m_pD3D = pImage->m_pD3D;
    m_vDiffuse = VECTOR4(1, 1, 1, 1); // -- 2020.1.24
    m_nBlend = 1;
    SetSrc(pImage, srcX, srcY, srcwidth, srcheight, srcwidth, srcheight);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃R���X�g���N�^	
//
//  CSpriteImage*        pImage      �X�v���C�g�C���[�W�|�C���^
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//  const DWORD&         destwidth   �\���̕�
//  const DWORD&         destheight  �\���̍���
//
//------------------------------------------------------------------------
CSprite::CSprite(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth,
                 const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight)
{
    ZeroMemory(this, sizeof(CSprite));
    m_pShader = pImage->m_pShader;
    m_pD3D = pImage->m_pD3D;
    m_vDiffuse = VECTOR4(1, 1, 1, 1); // -- 2020.1.24
    m_nBlend = 1;
    SetSrc(pImage, srcX, srcY, srcwidth, srcheight, destwidth, destheight);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CSprite::~CSprite()
{
    SAFE_RELEASE(m_pVertexBufferSprite);
    SAFE_RELEASE(m_pVertexBufferLine);
    SAFE_RELEASE(m_pVertexBufferRect);
    SAFE_RELEASE(m_pVertexBufferBillSprite); // 3DSprite
}

//------------------------------------------------------------------------
//                                                         // -- 2017.10.9
//	�X�v���C�g�̃C���[�W��ݒ肷��	
//
//  CSpriteImage* pImage      �X�v���C�g�C���[�W�|�C���^
//
//------------------------------------------------------------------------
void CSprite::SetImage(CSpriteImage* pImage)
{
    m_pImage = pImage;
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃C���[�W��ʒu�A�傫���̏���ݒ肷��	
//
//  CSpriteImage*        pImage      �X�v���C�g�C���[�W�|�C���^
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth,
                        const DWORD& srcheight)
{
    m_pImage = pImage;
    return SetSrc(srcX, srcY, srcwidth, srcheight, srcwidth, srcheight);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃C���[�W��ʒu�A�傫���̏���ݒ肷��	
//
//  CSpriteImage*        pImage      �X�v���C�g�C���[�W�|�C���^
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//  const DWORD&         destwidth   �\���̕�
//  const DWORD&         destheight  �\���̍���
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc(CSpriteImage* pImage, const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth,
                        const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight)
{
    m_pImage = pImage;
    return SetSrc(srcX, srcY, srcwidth, srcheight, destwidth, destheight);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃C���[�W��ʒu�A�傫���̏���ݒ肷��	
//
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight)
{
    return SetSrc(srcX, srcY, srcwidth, srcheight, srcwidth, srcheight);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g�̃C���[�W��ʒu�A�傫���̏���ݒ肷��	
//
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//  const DWORD&         destwidth   �\���̕�
//  const DWORD&         destheight  �\���̍���
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight,
                        const DWORD& destwidth, const DWORD& destheight)
{
    // �X�v���C�g�p�̔|���S�����쐬���A�o�[�e�b�N�X�o�b�t�@���쐬����
    m_dwSrcX = srcX;
    m_dwSrcY = srcY;
    m_dwSrcWidth = srcwidth;
    m_dwSrcHeight = srcheight;
    m_dwDestWidth = destwidth;
    m_dwDestHeight = destheight;

    //�o�[�e�b�N�X�o�b�t�@�[�쐬
    //�C�����邱�ƁBz�l���P�ȏ�ɂ��Ȃ��B�N���b�v��Ԃ�z=1�͍ł������Ӗ�����B���������ĕ`�悳��Ȃ��B
    SpriteVertex vertices[] =
    {
        VECTOR3(0, (float)m_dwDestHeight, 0),
        VECTOR2((float)m_dwSrcX / m_pImage->m_dwImageWidth,
                (float)(m_dwSrcY + m_dwSrcHeight) / m_pImage->m_dwImageHeight), //���_1  ����
        VECTOR3(0, 0, 0),
        VECTOR2((float)m_dwSrcX / m_pImage->m_dwImageWidth, (float)m_dwSrcY / m_pImage->m_dwImageHeight),
        //���_2�@����  // -- 2024.3.23
        VECTOR3((float)m_dwDestWidth, (float)m_dwDestHeight, 0),
        VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_pImage->m_dwImageWidth,
                (float)(m_dwSrcY + m_dwSrcHeight) / m_pImage->m_dwImageHeight), //���_3�@�E��  // -- 2024.3.23
        VECTOR3((float)m_dwDestWidth, 0, 0),
        VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_pImage->m_dwImageWidth,
                (float)m_dwSrcY / m_pImage->m_dwImageHeight), //���_4�@�E��
    };

    // �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
    if (m_pVertexBufferSprite == nullptr)
    {
        // �V�K�쐬����
        D3D11_BUFFER_DESC bd;
        //bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bd.CPUAccessFlags = 0;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferSprite)))
        {
            MessageBox(0, _T("Sprite.cpp �o�[�e�b�N�X�o�b�t�@�[�쐬���s"), nullptr, MB_OK);
            return E_FAIL;
        }
    }
    else
    {
        // ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4); // 4���_���R�s�[
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferSprite, 0);
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------
//
//	�X�v���C�g����ʂɃ����_�����O
//
//  CSpriteImage* pImage      �X�v���C�g�C���[�W�|�C���^
//  const float&         posX        �\���ʒu�̍���@�w���W
//  const float&         posY        �\���ʒu�̍���@�x���W
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//  const float&         fAlpha      �����x
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY,
                   const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha)
{
    SetSrc(pImage, srcX, srcY, srcwidth, srcheight);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha; // -- 2020.1.24
    Draw(posX, posY);
}

//------------------------------------------------------------------------
//
//  スプライト描画
//
//  CSpriteImage* pImage      スプライト画像ポインタ
//  const float&         posX        表示位置のX座標
//  const float&         posY        表示位置のY座標
//  const DWORD&         srcX        パターンのX座標
//  const DWORD&         srcY        パターンのY座標
//  const DWORD&         srcwidth    パターンの幅
//  const DWORD&         srcheight   パターンの高さ
//  const DWORD&         destwidth   表示の幅
//  const DWORD&         destheight  表示の高さ
//  const float&         fAlpha      透過度
//
//  戻り値  なし
//------------------------------------------------------------------------
void CSprite::Draw(CSpriteImage* pImage, const float& posX, const float& posY, const DWORD& srcX, const DWORD& srcY,
                   const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight,
                   const float& fAlpha)
{
    SetSrc(pImage, srcX, srcY, srcwidth, srcheight, destwidth, destheight);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha; // -- 2020.1.24
    Draw(posX, posY);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g����ʂɃ����_�����O
//
//  CSpriteImage* pImage      �X�v���C�g�C���[�W�|�C���^
//  const MATRIX4X4&    mWorld      �\���ʒu�̃��[���h�}�g���b�N�X
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//  const float&         fAlpha      �����x
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY,
                   const DWORD& srcwidth, const DWORD& srcheight, const float& fAlpha)
{
    SetSrc(pImage, srcX, srcY, srcwidth, srcheight);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha; // -- 2020.1.24
    Draw(mWorld);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g����ʂɃ����_�����O
//
//  CSpriteImage* pImage      �X�v���C�g�C���[�W�|�C���^
//  const MATRIX4X4&    mWorld      �\���ʒu�̃��[���h�}�g���b�N�X
//  const DWORD&         srcX        �p�^�[���̍���@�w���W
//  const DWORD&         srcY        �p�^�[���̍���@�x���W
//  const DWORD&         srcwidth    �p�^�[���̕�
//  const DWORD&         srcheight   �p�^�[���̍���
//  const DWORD&         destwidth   �\���̕�
//  const DWORD&         destheight  �\���̍���
//  const float&         fAlpha      �����x
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::Draw(CSpriteImage* pImage, const MATRIX4X4& mWorld, const DWORD& srcX, const DWORD& srcY,
                   const DWORD& srcwidth, const DWORD& srcheight, const DWORD& destwidth, const DWORD& destheight,
                   const float& fAlpha)
{
    SetSrc(pImage, srcX, srcY, srcwidth, srcheight, destwidth, destheight);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha; // -- 2020.1.24
    Draw(mWorld);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g����ʂɃ����_�����O
//
//  const float&         posX     �\���ʒu�̍���@�w���W
//  const float&         posY     �\���ʒu�̍���@�x���W
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::Draw(const float& posX, const float& posY)
{
    MATRIX4X4 mWorld;

    mWorld = XMMatrixTranslation(posX, posY, 0.0f);
    Draw(mWorld);
}

//------------------------------------------------------------------------
//
//	�X�v���C�g����ʂɃ����_�����O�@�T�u�֐�
//
//  const MATRIX4X4&    mWorld   �\���ʒu�̃��[���h�}�g���b�N�X
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::Draw(const MATRIX4X4& mWorld)
{
    //�g�p����V�F�[�_�[�̃Z�b�g
    SetShader();

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferSprite, &stride, &offset);

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        //���[���h�s���n��
        cb.mW = XMMatrixTranspose(mWorld);

        //�r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
        cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
        cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
        cb.vUVOffset.x = (float)m_ofX / m_pImage->m_dwImageWidth;
        cb.vUVOffset.y = (float)m_ofY / m_pImage->m_dwImageHeight;
        cb.vColor = m_vDiffuse; // -- 2020.1.24
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // �e�N�X�`���L��
        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    //�e�N�X�`���[���V�F�[�_�[�ɓn��
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);

    //�v���~�e�B�u�������_�����O
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    //	�V�F�[�_�[�������Z�b�g
    ResetShader();
}

//------------------------------------------------------------------------
//
//	���C������ʂɃ����_�����O�@�T�u�֐�                                         // -- 2017.10.9
//
//  �i�Ȃ��A�F�ⓧ���x��m_vDiffuse���g�p���Ă��Ȃ��B�����ɂ�钼�ڎw��ł���j
//
//  const float& StartX     ���C���`��@�J�n�w���W
//  const float& StartY     ���C���`��@�J�n�x���W
//  const float& EndX       ���C���`��@�I���w���W
//  const float& EndY       ���C���`��@�I���x���W
//  const DWORD& WidthIn    ���C���̑����i�P�ȏ�j
//  const DWORD& colorABGR  ���F�@colorABGR�́AABGR�̎w��B��F���F��(0x00ffffff)  ������RGB(1,1,1)
//  const float& fAlpha     �����x�i�ȗ��l��1.0f�j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::DrawLine(const float& StartX, const float& StartY, const float& EndX, const float& EndY,
                       const DWORD& WidthIn, const DWORD& colorABGR, const float& fAlpha)
{
    DWORD Width = WidthIn; // ���C���̑���

    // ������ʂ͈͓̔��ɓ����Ă��Ȃ�������`�悵�Ȃ�
    if ((StartX < 0 && EndX < 0) || (StartY < 0 && EndY < 0) ||
        (StartX > m_pD3D->m_dwWindowWidth && EndX > m_pD3D->m_dwWindowWidth) ||
        (StartY > m_pD3D->m_dwWindowHeight && EndY > m_pD3D->m_dwWindowHeight))
    {
        return;
    }

    //�g�p����V�F�[�_�[�̃Z�b�g
    SetShader();

    // ���|���S���o�[�e�b�N�X�o�b�t�@�[�쐬
    SpriteVertex vertices[] =
    {
        {VECTOR3(StartX, StartY, 0), VECTOR2(0, 0)}, //���_1
        {VECTOR3(EndX, EndY, 0), VECTOR2(0, 0)} //���_2
    };

    // �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
    if (m_pVertexBufferLine == nullptr)
    {
        // �V�K�쐬����
        D3D11_BUFFER_DESC bd;
        //bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 2;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bd.CPUAccessFlags = 0;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferLine)))
        {
            MessageBox(0, _T("Sprite.cpp �o�[�e�b�N�X�o�b�t�@�[LINE �쐬���s"), nullptr, MB_OK);
            return;
        }
    }
    else
    {
        // ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferLine, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 2); // 2���_���R�s�[
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferLine, 0);
        }
    }

    VECTOR4 color;
    color.x = ((colorABGR & 0x000000ff) >> 0) / (float)255; // R
    color.y = ((colorABGR & 0x0000ff00) >> 8) / (float)255; // G
    color.z = ((colorABGR & 0x00ff0000) >> 16) / (float)255; // B
    color.w = fAlpha; // A

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferLine, &stride, &offset);

    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //�e�N�X�`���[�Ȃ����V�F�[�_�[�ɓn��
    ID3D11ShaderResourceView* Nothing[1] = {0};
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

    //���[���h�s��
    MATRIX4X4 mWorld;

    // �����̖@�������߂�
    // (���̑����������Â炵�ĕ`�悷�邽�߂ɂ��̕`����������߂邽��)
    VECTOR2 vNrm, vDif = VECTOR2(EndX - StartX, EndY - StartY), vLen;
    vNrm.x = vDif.y;
    vNrm.y = vDif.x * -1;

    vLen = XMVector2Length(vNrm);
    vNrm.x = vNrm.x / vLen.x;
    vNrm.y = vNrm.y / vLen.x;

    if (Width < 1) Width = 1;

    // ���̕������`����J��Ԃ�
    for (DWORD i = 0; i < Width; i++)
    {
        // ���[���h�s��̏�����
        mWorld = XMMatrixIdentity();

        // ���̑������Â炵�ĕ\�����邽�߂̏���
        if (i % 2 == 0)
        {
            mWorld._41 = vNrm.x * 0.8f * i * 0.5f;
            mWorld._42 = vNrm.y * 0.8f * i * 0.5f;
        }
        else
        {
            mWorld._41 = -vNrm.x * 0.8f * i * 0.5f;
            mWorld._42 = -vNrm.y * 0.8f * i * 0.5f;
        }

        //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
        D3D11_MAPPED_SUBRESOURCE pData;
        CONSTANT_BUFFER_SPRITE cb;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
        {
            //���[���h�s���n��
            cb.mW = XMMatrixTranspose(mWorld);

            //�r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
            cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
            cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
            cb.vUVOffset.x = 0;
            cb.vUVOffset.y = 0;
            cb.vColor = color;
            cb.vMatInfo = VECTOR4(0, 0, 0, 0); // �e�N�X�`���Ȃ�
            memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
            m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
        }
        //�v���~�e�B�u�������_�����O
        m_pD3D->m_pDeviceContext->Draw(2, 0);
    }

    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //	�V�F�[�_�[�������Z�b�g
    ResetShader();
}

//------------------------------------------------------------------------
//                                                         // -- 2018.3.20
//	�l�p�`����ʂɃ����_�����O�@�T�u�֐�
//
//  �i�Ȃ��A�F�ⓧ���x��m_vDiffuse���g�p���Ă��Ȃ��B�����ɂ�钼�ڎw��ł���j
//
//  const float& posX       �l�p�`�`��@�J�n�w���W
//  const float& posY       �l�p�`�`��@�J�n�x���W
//  const DWORD& width      �l�p�`�̕�
//  const DWORD& height     �l�p�`�̍���
//  const DWORD& colorABGR  �`��F�@colorABGR�́AABGR�̎w��B��F���F��(0x00ffffff)   ������RGB(1,1,1)
//  const float& fAlpha     �����x�i�ȗ��l��1.0f�j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::DrawRect(const float& posX, const float& posY, const DWORD& width, const DWORD& height,
                       const DWORD& colorABGR, const float& fAlpha)
{
    //�g�p����V�F�[�_�[�̃Z�b�g
    SetShader();

    // �o�[�e�b�N�X�o�b�t�@�[�쐬
    SpriteVertex vertices[] =
    {
        {VECTOR3(0, (float)height, 0), VECTOR2(0, 0)}, //���_1  ����
        {VECTOR3(0, 0, 0), VECTOR2(0, 0)}, //���_3�@����    // -- 2024.3.23
        {VECTOR3((float)width, (float)height, 0), VECTOR2(0, 0)}, //���_2�@�E��    // -- 2024.3.231
        {VECTOR3((float)width, 0, 0), VECTOR2(0, 0)}, //���_4�@�E��
    };

    // �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
    if (m_pVertexBufferRect == nullptr)
    {
        // �V�K�쐬����
        D3D11_BUFFER_DESC bd;
        //bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4; // 4���_��
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bd.CPUAccessFlags = 0;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferRect)))
        {
            MessageBox(0, _T("Sprite.cpp �o�[�e�b�N�X�o�b�t�@�[RECT �쐬���s"), nullptr, MB_OK);
            return;
        }
    }
    else
    {
        // ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferRect, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4); // 4���_���R�s�[
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferRect, 0);
        }
    }

    VECTOR4 color;
    color.x = ((colorABGR & 0x000000ff) >> 0) / (float)255; // R
    color.y = ((colorABGR & 0x0000ff00) >> 8) / (float)255; // G
    color.z = ((colorABGR & 0x00ff0000) >> 16) / (float)255; // B
    color.w = fAlpha; // A

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferRect, &stride, &offset);

    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //�e�N�X�`���[�Ȃ����V�F�[�_�[�ɓn��
    ID3D11ShaderResourceView* Nothing[1] = {0};
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        //���[���h�s���n��
        cb.mW = XMMatrixTranspose(XMMatrixTranslation(posX, posY, 0.0f));

        //�r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
        cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
        cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
        cb.vUVOffset.x = 0;
        cb.vUVOffset.y = 0;
        cb.vColor = color;
        cb.vMatInfo = VECTOR4(0, 0, 0, 0); // �e�N�X�`���Ȃ�
        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }
    //�v���~�e�B�u�������_�����O
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    //	�V�F�[�_�[�������Z�b�g
    ResetShader();
}

void CSprite::DrawCircle(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcWid,
                         DWORD srcHei, float startRad,float endRad, float fAlpha)
{
    // 円形プログレスバー用に、UV座標を0～1の範囲に設定
    m_pImage = pImage;

    // UV座標を0～1に固定（角度計算用）
    SpriteVertex vertices[] =
    {
        VECTOR3(0, static_cast<float>(srcHei), 0), VECTOR2(0.0f, 1.0f),  // 左上
        VECTOR3(0, 0, 0), VECTOR2(0.0f, 0.0f),              // 左下
        VECTOR3(static_cast<float>(srcWid), static_cast<float>(srcHei), 0), VECTOR2(1.0f, 1.0f),  // 右上
        VECTOR3(static_cast<float>(srcWid), 0, 0), VECTOR2(1.0f, 0.0f),  // 右下
    };

    // 頂点バッファを更新
    if (m_pVertexBufferSprite)
    {
        D3D11_MAPPED_SUBRESOURCE mapResource;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource)))
        {
            memcpy_s(mapResource.pData, sizeof(vertices), vertices, sizeof(vertices));
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferSprite, 0);
        }
    }

    //シェーダー設定
    SetShader();

    //頂点バッファ
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    //ワールド行列作成
    MATRIX4X4 mWorld = XMMatrixTranslation(posX, posY, 0.0f);
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferSprite, &stride, &offset);

    //シェーダーのコンスタントバッファーに各種データを渡す
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE     cb;
    if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(
        m_pShader->m_pConstantBufferSprite3D,0,D3D11_MAP_WRITE_DISCARD,0,&pData)))
    {
        cb.mW = XMMatrixTranspose(mWorld);
        cb.ViewPortWidth = static_cast<float>(m_pD3D->m_dwWindowWidth);
        cb.ViewPortHeight = static_cast<float>(m_pD3D->m_dwWindowHeight);

        //UV offset
        cb.vUVOffset.x = 0;
        cb.vUVOffset.y = 0;
        
        //color
        cb.vColor = VECTOR4(1,1,1,fAlpha);
        
        //円の角度設定
        cb.vMatInfo.x = 1;      //　テクスチャあり
        cb.vMatInfo.y = startRad;   //　開始角度
        cb.vMatInfo.z = endRad;    //　終了角度
        cb.vMatInfo.w = 1;      //　円モードon
        
        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }
    
    //テクスチャーをシェーダーに渡す
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);

    //描画
    m_pD3D->m_pDeviceContext->Draw(4, 0);
    ResetShader();
}


//------------------------------------------------------------------------
//
//	�`��O�ɃV�F�[�_�[�����Z�b�g
//
//  �����@�@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::SetShader()
{
    //�g�p����V�F�[�_�[�̃Z�b�g
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);


    //���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    //���_�C���v�b�g���C�A�E�g���Z�b�g
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);

    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // �T���v���[���Z�b�g
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

    // �u�����f�B���O��ݒ�            // -- 2019.8.18
    UINT mask = 0xffffffff;
    if (m_nBlend == 1)
    {
        // 1:���ߐF�̃u�����f�B���O��ݒ�
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
    }
    else if (m_nBlend == 2)
    {
        // 2:���Z�����̃u�����f�B���O��ݒ�
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, nullptr, mask);
    }

    //Z�o�b�t�@�𖳌���
    m_pD3D->SetZBuffer(false); // -- 2019.4.19
}

//------------------------------------------------------------------------
//
//	�`���ɃV�F�[�_�[�������Z�b�g
//
//  �����@�@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CSprite::ResetShader()
{
    //Z�o�b�t�@��L����
    m_pD3D->SetZBuffer(true); // -- 2019.4.19


    // �ʏ�̃u�����f�B���O�ɖ߂�
    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
}


//------------------------------------------------------------------------ // -- 2018.8.10
//
//	�R�c�i�r���{�[�h�j�X�v���C�g�̃o�[�e�B�N�X�o�b�t�@�쐬	
//
//  CSpriteImage* pImage
//  const float& fDestWidth         �\����
//  const float& fDestHeight        �\������
//  const DWORD& dwSrcX
//  const DWORD& dwSrcY
//  const DWORD& dwSrcWidth
//  const DWORD& dwSrcHeigh
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc3D(CSpriteImage* pImage, const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX,
                          const DWORD& dwSrcY, const DWORD& dwSrcWidth, const DWORD& dwSrcHeight)
{
    m_pImage = pImage;
    return SetSrc3D(fDestWidth, fDestHeight, dwSrcX, dwSrcY, dwSrcWidth, dwSrcHeight);
}

//------------------------------------------------------------------------ // -- 2018.9.30
//
//	�R�c�i�r���{�[�h�j�X�v���C�g�̃o�[�e�B�N�X�o�b�t�@�쐬	
//
//  const float& fDestWidth         �\����
//  const float& fDestHeight        �\������
//  const DWORD& dwSrcX
//  const DWORD& dwSrcY
//  const DWORD& dwSrcWidth
//  const DWORD& dwSrcHeigh
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc3D(const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX, const DWORD& dwSrcY,
                          const DWORD& dwSrcWidth, const DWORD& dwSrcHeight)
{
    // �r���{�[�h�e�N�X�`���p�̔|���S�����쐬���A�o�[�e�b�N�X�o�b�t�@���쐬����
    m_dwSrcX = dwSrcX;
    m_dwSrcY = dwSrcY;
    m_dwSrcWidth = dwSrcWidth;
    m_dwSrcHeight = dwSrcHeight;
    m_fDestWidth = fDestWidth;
    m_fDestHeight = fDestHeight;

    DWORD dwImageWidth = m_pImage->m_dwImageWidth;
    DWORD dwImageHeight = m_pImage->m_dwImageHeight;

    //�o�[�e�b�N�X�o�b�t�@�[�쐬(�������̍��W�ō쐬�B��_�̓|���S���̒��S�_)
    SpriteVertex vertices[] =
    {
        VECTOR3(-m_fDestWidth / 2, m_fDestHeight / 2, 0),
        VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / dwImageWidth, (float)m_dwSrcY / dwImageHeight), //���_1  ����
        VECTOR3(-m_fDestWidth / 2, -m_fDestHeight / 2, 0),
        VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / dwImageWidth, (float)(m_dwSrcY + m_dwSrcHeight) / dwImageHeight),
        //���_2�@����	// -- 2024.3.23
        VECTOR3(m_fDestWidth / 2, m_fDestHeight / 2, 0),
        VECTOR2((float)m_dwSrcX / dwImageWidth, (float)m_dwSrcY / dwImageHeight), //���_3�@�E��	// -- 2024.3.2
        VECTOR3(m_fDestWidth / 2, -m_fDestHeight / 2, 0),
        VECTOR2((float)m_dwSrcX / dwImageWidth, (float)(m_dwSrcY + m_dwSrcHeight) / dwImageHeight), //���_4�@�E��
    };

    // �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
    if (m_pVertexBufferBillSprite == nullptr)
    {
        // �V�K�쐬����
        D3D11_BUFFER_DESC bd;
        //bd.Usage = D3D11_USAGE_DEFAULT;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bd.CPUAccessFlags = 0;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferBillSprite)))
        {
            return E_FAIL;
        }
    }
    else
    {
        // ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferBillSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4); // 4���_���R�s�[
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferBillSprite, 0);
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------ // -- 204.3.23
//
//	�R�c�i�r���{�[�h�j�X�v���C�g�I�u�W�F�N�g����ʂɃ����_�����O	
//
// -----------------------------------------------------------------------
bool CSprite::Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos,
                     const VECTOR2& vSrcSize, const float& fAlpha) // -- 2024.3.23
{
    return Draw3D(pImage, vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, vSize, vSrcPos,
                  vSrcSize, fAlpha);
}

bool CSprite::Draw3D(const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize,
                     const float& fAlpha) // -- 2024.3.23
{
    return Draw3D(vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, vSize, vSrcPos, vSrcSize,
                  fAlpha);
}

bool CSprite::Draw3D(const VECTOR3& vPos) // -- 2024.3.23
{
    return Draw3D(vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt);
}

bool CSprite::DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const DWORD& colorABGR, const float& fAlpha)
// -- 2024.3.23
{
    return DrawLine3D(vStart, vEnd, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, colorABGR,
                      fAlpha);
}

bool CSprite::Draw3DWithWorldMatrix(CSpriteImage* pImage, const MATRIX4X4& mWorld, const MATRIX4X4& mView,
                                    const MATRIX4X4& mProj, const VECTOR2& vSize, const VECTOR2& vSrcPos,
                                    const VECTOR2& vSrcSize, const float& fAlpha)
{
    m_pImage = pImage;

    if (m_pImage == nullptr) return false;

    // バーテックスバッファを作成
    SetSrc3D(vSize.x, vSize.y, (DWORD)vSrcPos.x, (DWORD)vSrcPos.y, (DWORD)vSrcSize.x, (DWORD)vSrcSize.y);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha;

    if (m_pVertexBufferBillSprite == nullptr) return false;

    // シェーダーのセット (ビルボード用ではなく通常の頂点シェーダーを使用)
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    // バーテックスバッファーをセット
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferBillSprite, &stride, &offset);

    // ブレンディングの設定
    UINT mask = 0xffffffff;
    if (m_nBlend == 1)
    {
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
    }
    else if (m_nBlend == 2)
    {
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, nullptr, mask);
    }

    // シェーダーのコンスタントバッファーに各種データを渡す
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    ZeroMemory(&cb, sizeof(cb));

    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        // ★ ここがポイント: ビルボード行列ではなく、指定されたワールド行列を使用
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        cb.vUVOffset.x = (float)m_ofX / m_pImage->m_dwImageWidth;
        cb.vUVOffset.y = (float)m_ofY / m_pImage->m_dwImageHeight;
        cb.vColor = m_vDiffuse;
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // テクスチャ有効

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    // このコンスタントバッファーをどのシェーダーで使うか
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    // 頂点インプットレイアウトをセット
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);
    // プリミティブ・トポロジーをセット
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    // テクスチャーをシェーダーに渡す
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);
    // プリミティブをレンダリング
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // 通常のブレンディングに戻す
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    return true;
}

//------------------------------------------------------------------------ // -- 2018.8.10
//
//	�R�c�i�r���{�[�h�j�X�v���C�g�I�u�W�F�N�g����ʂɃ����_�����O	
//
//	�o�[�e�b�N�X�o�b�t�@���쐬���ĕ`�������
//
//	����
//		CSpriteImage* pImage   �X�v���C�g�C���[�W
//		const VECTOR3& vPos       �\���ʒu
//		const MATRIX4X4& mView       �r���[�}�g���b�N�X
//		const MATRIX4X4& mProj       �v���W�F�N�V�����}�g���b�N�X
//		const VECTOR3& vEye       ���_�ʒu
//		const VECTOR2& vSize      �\���T�C�Y
//		const VECTOR2& vSrcPos    �p�^�[���̈ʒu
//		const VECTOR2& vSrcSize   �p�^�[���̑傫��
//		const float& fAlpha           �����x�i�ȗ��j
//
//	�߂�l bool
//		true      �\���p��
//		false     �\���I��
//
//------------------------------------------------------------------------
bool CSprite::Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                     const VECTOR3& vEye, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize,
                     const float& fAlpha)
{
    m_pImage = pImage;

    return Draw3D(vPos, mView, mProj, vEye, vSize, vSrcPos, vSrcSize, fAlpha);
}

//------------------------------------------------------------------------ // -- 2019.8.18
//
//	�R�c�i�r���{�[�h�j�X�v���C�g�I�u�W�F�N�g����ʂɃ����_�����O	
//
//	�o�[�e�b�N�X�o�b�t�@���쐬���ĕ`�������
//
//	����
//		const VECTOR3& vPos       �\���ʒu
//		const MATRIX4X4& mView    �r���[�}�g���b�N�X
//		const MATRIX4X4& mProj    �v���W�F�N�V�����}�g���b�N�X
//		const VECTOR3& vEye       ���_�ʒu
//		const VECTOR2& vSize      �\���T�C�Y
//		const VECTOR2& vSrcPos    �p�^�[���̈ʒu
//		const VECTOR2& vSrcSize   �p�^�[���̑傫��
//		const float& fAlpha       �����x�i�ȗ��j
//
//	�߂�l bool
//		true      �\���p��
//		false     �\���I��
//
//------------------------------------------------------------------------
bool CSprite::Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye,
                     const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha)
{
    // �C���[�W���Ȃ��Ƃ��͕`�悵�Ȃ�
    if (m_pImage == nullptr) return false;

    // �\���r���{�[�h�̑傫���̃o�[�e�b�N�X�o�b�t�@���쐬����
    SetSrc3D(vSize.x, vSize.y, (DWORD)vSrcPos.x, (DWORD)vSrcPos.y, (DWORD)vSrcSize.x, (DWORD)vSrcSize.y);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha; // -- 2020.1.24
    return Draw3D(vPos, mView, mProj, vEye);
}

//------------------------------------------------------------------------ // -- 2018.8.10
//
//	�R�c�i�r���{�[�h�j�X�v���C�g�I�u�W�F�N�g����ʂɃ����_�����O	
//
//	���ɍ쐬�ς݂̃o�[�e�b�N�X�o�b�t�@���g�p���ĕ`�������
//
//	����
//		const VECTOR3&   vPos       �\���ʒu
//		const MATRIX4X4& mView      �r���[�}�g���b�N�X
//		const MATRIX4X4& mProj      �v���W�F�N�V�����}�g���b�N�X
//		const VECTOR3&   vEye       ���_�ʒu
//
//	�߂�l bool
//		true      �\���p��
//		false     �\���I��
//
//------------------------------------------------------------------------
bool CSprite::Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye)
{
    // �C���[�W���Ȃ�������o�[�e�b�N�X�o�b�t�@���쐬����Ă��Ȃ��Ƃ��͕`�悵�Ȃ�
    if (m_pImage == nullptr || m_pVertexBufferBillSprite == nullptr) return false;

    //�r���{�[�h�́A���_���������[���h�g�����X�t�H�[�������߂�
    MATRIX4X4 mWorld = GetLookatMatrix(vPos, vEye);

    //�g�p����V�F�[�_�[�̃Z�b�g
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferBillSprite, &stride, &offset);

    // �u�����f�B���O��ݒ�            // -- 2019.8.18
    UINT mask = 0xffffffff;
    if (m_nBlend == 1)
    {
        // 1:���ߐF�̃u�����f�B���O��ݒ�
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
    }
    else if (m_nBlend == 2)
    {
        // 2:���Z�����̃u�����f�B���O��ݒ�
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, nullptr, mask);
    }

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    ZeroMemory(&cb, sizeof(cb));

    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        //���[���h�A�J�����A�ˉe�s��A�e�N�X�`���[�I�t�Z�b�g��n��
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        cb.vUVOffset.x = (float)m_ofX / m_pImage->m_dwImageWidth; // �p�^�[���̈ʒu�w�����̍����i�e�N�X�`�����W�j
        cb.vUVOffset.y = (float)m_ofY / m_pImage->m_dwImageHeight; // �p�^�[���̈ʒu�x�����̍����i�e�N�X�`�����W�j
        cb.vColor = m_vDiffuse; // -- 2020.1.24
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // �e�N�X�`���L��

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }
    //���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    //���_�C���v�b�g���C�A�E�g���Z�b�g
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);
    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    //�e�N�X�`���[���V�F�[�_�[�ɓn��
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);
    //�v���~�e�B�u�������_�����O
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // �ʏ�̃u�����f�B���O�ɖ߂�
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    return true;
}

//------------------------------------------------------------------------ // -- 2018.8.10
//
//	�R�c���C������ʂɃ����_�����O	
//
//	���ɍ쐬�ς݂̃o�[�e�b�N�X�o�b�t�@���g�p���ĕ`�������
//
//  �i�Ȃ��A�F�ⓧ���x��m_vDiffuse���g�p���Ă��Ȃ��B�����ɂ�钼�ڎw��ł���j
//
//	����
//		const VECTOR3& vStart     ���C���̎n�_�ʒu
//		const VECTOR3& vEnd       ���C���̏I�_�ʒu
//		const MATRIX4X4& mView    �r���[�}�g���b�N�X
//		const MATRIX4X4& mProj    �v���W�F�N�V�����}�g���b�N�X
//		const VECTOR3& vEye       ���_�ʒu
//		const DWORD& colorABGR    ���F�@colorABGR�́AABGR�̎w��B��F���F��(0x00ffffff)  ������RGB(255,255,255)
//		const float& fAlpha       �����x(�ȗ��l�P)
//
//	�߂�l bool
//		true      �\���p��
//		false     �\���I��
//
//------------------------------------------------------------------------
bool CSprite::DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                         const VECTOR3& vEye, const DWORD& colorABGR, const float& fAlpha)
{
    //�g�p����V�F�[�_�[�̃Z�b�g
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    // ���|���S���o�[�e�b�N�X�o�b�t�@�[�쐬
    SpriteVertex vertices[] =
    {
        {VECTOR3(vStart.x, vStart.y, vStart.z), VECTOR2(0, 0)}, //���_1
        {VECTOR3(vEnd.x, vEnd.y, vEnd.z), VECTOR2(0, 0)} //���_2
    };

    // �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
    if (m_pVertexBufferLine == nullptr)
    {
        // �V�K�쐬����
        D3D11_BUFFER_DESC bd;
        //bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 2;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bd.CPUAccessFlags = 0;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferLine)))
        {
            MessageBox(0, _T("Sprite3D.cpp �o�[�e�b�N�X�o�b�t�@�[LINE �쐬���s"), nullptr, MB_OK);
            return false;
        }
    }
    else
    {
        // ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferLine, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 2); // 2���_���R�s�[
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferLine, 0);
        }
    }

    // ���F�̎w��
    VECTOR4 color;
    color.x = ((colorABGR & 0x000000ff) >> 0) / (float)255; // R
    color.y = ((colorABGR & 0x0000ff00) >> 8) / (float)255; // G
    color.z = ((colorABGR & 0x00ff0000) >> 16) / (float)255; // B
    color.w = fAlpha; // A    // -- 2020.1.24

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferLine, &stride, &offset);

    //���_�C���v�b�g���C�A�E�g���Z�b�g
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);

    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    //�e�N�X�`���[�Ȃ����V�F�[�_�[�ɓn��
    ID3D11ShaderResourceView* Nothing[1] = {0};
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

    //���[���h�s��
    MATRIX4X4 mWorld = XMMatrixIdentity();

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        //���[���h�A�J�����A�ˉe�s��A�e�N�X�`���[�I�t�Z�b�g��n��
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        cb.vUVOffset.x = 0;
        cb.vUVOffset.y = 0;
        cb.vColor = color; // ���F
        cb.vMatInfo = VECTOR4(0, 0, 0, 0); // �e�N�X�`���Ȃ�

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    //���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);

    //�v���~�e�B�u�������_�����O
    m_pD3D->m_pDeviceContext->Draw(2, 0);

    return true;
}

// ========================================================================================
//
// �t�H���g�e�N�X�`���[�̏���
//
//
// ========================================================================================
//------------------------------------------------------------------------
//
//	�t�H���g�e�N�X�`���[�̃R���X�g���N�^	
//
//  �����@CShader* pShader
//
//------------------------------------------------------------------------
CFontTexture::CFontTexture(CShader* pShader)
{
    ZeroMemory(this, sizeof(CFontTexture));
    m_pShader = pShader;
    m_pD3D = pShader->m_pD3D;
    m_Idx = 0;
    for (DWORD i = 0; i < TEXT_DATA_MAX; i++)
    {
        m_TextData[i].m_fAlpha = 1.0f;
    }

    /*
    // �����Œǉ������t�H���g���ꎞ�I�Ɏg����悤�ɂ���
    // �v���O�������I������ƃt�H���g�͍폜�����
    DESIGNVECTOR design;
    if (AddFontResourceEx(
    TEXT("Fonts/�ǉ��t�H���g��.ttf"),
    FR_PRIVATE,
    &design) == nullptr)
    {
    MessageBox(0, _T("Sprite.cpp �t�H���g�̒ǉ��Ɏ��s���܂���"), _T(""), MB_OK);
    }
    */

    // 3D�e�L�X�g   // -- 2018.8.10
    m_fDestWidth = 0.0f;
    m_fDestHeight = 0.0f;
}

//------------------------------------------------------------------------
//
//	�t�H���g�e�N�X�`���[�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CFontTexture::~CFontTexture()
{
    /*
    // �ǉ������t�H���g���������
    DESIGNVECTOR design;
    if (RemoveFontResourceEx(
    TEXT("Fonts/�ǉ��t�H���g��.ttf"),
    FR_PRIVATE,
    &design) == nullptr)
    {
    MessageBox(0, _T("Sprite.cpp �t�H���g�̉���Ɏ��s���܂���"), _T(""), MB_OK);
    }
    */

    for (DWORD i = 0; i < TEXT_DATA_MAX; i++)
    {
        SAFE_DELETE_ARRAY(m_TextData[i].m_szText);
        SAFE_RELEASE(m_TextData[i].m_pResourceView);
        SAFE_RELEASE(m_TextData[i].m_pVertexBufferFont);
    }
}

//------------------------------------------------------------------------
// �e�L�X�g���̃��t���b�V��
// 
// �@�@�\������e�L�X�g�̏��́A��s�Â�TextData�֕ۑ������
// �@�@���̃��t���b�V���́A���C�����[�v�̐擪�ōs���K�v������
//
//  �����@�@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CFontTexture::Refresh()
{
    m_Idx = 0;
}

//------------------------------------------------------------------------
//
//	�e�L�X�g�\���p�̃o�[�e�b�N�X�o�b�t�@�[�쐬�B
//	�P�s���̑傫�����m�ۂ���
//
//  const DWORD& dwWidth    �\�����i�P�s���̑傫���j
//  const DWORD& dwHeight   �\�������i�P�����̍����j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------ 
void CFontTexture::CreateVB(const DWORD& dwWidth, const DWORD& dwHeight)
{
    // �o�[�e�b�N�X�o�b�t�@�[�쐬���ɋC�����邱�ƁB
    // z�l���P�ȏ�ɂ��Ȃ����ƁB�N���b�v��Ԃ�z=1�͍ł������Ӗ�����B���������ĕ`�悳��Ȃ��B
    SpriteVertex vertices[] =
    {
        VECTOR3(0, (float)dwHeight, 0), VECTOR2(0, 1), //���_1  ����
        VECTOR3(0, 0, 0), VECTOR2(0, 0), //���_3�@����     // -- 2024.3.23
        VECTOR3((float)dwWidth, (float)dwHeight, 0), VECTOR2(1, 1), //���_2�@�E��     // -- 2024.3.23
        VECTOR3((float)dwWidth, 0, 0), VECTOR2(1, 0) //���_4�@�E��
    };

    // �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
    if (m_TextData[m_Idx].m_pVertexBufferFont == nullptr)
    {
        // �V�K�쐬����
        D3D11_BUFFER_DESC bd;
        //bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bd.CPUAccessFlags = 0;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_TextData[m_Idx].m_pVertexBufferFont)))
        {
            MessageBox(0, _T("Sprite.cpp �o�[�e�b�N�X�o�b�t�@�[�쐬���s"), nullptr, MB_OK);
            return;
        }
    }
    else
    {
        // ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_TextData[m_Idx].m_pVertexBufferFont, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4); // 4���_���R�s�[
            m_pD3D->m_pDeviceContext->Unmap(m_TextData[m_Idx].m_pVertexBufferFont, 0);
        }
    }
}

//------------------------------------------------------------------------
//
//	�e�L�X�g�̕`��
//
//	�E���W�w��̏������̏ꍇ
//
//  float posX         �\���ʒu�@����@�w���W
//  float posY         �\���ʒu�@����@�x���W
//  const TCHAR* szText       �\������e�L�X�g
//  int fontsize       �t�H���g�T�C�Y
//  DWORD colorABGR    �����F�@colorABGR�́AABGR�̎w��B��F���F��(0x00ffffff)
//  float fAlpha       �����x�i�ȗ��j
//  const TCHAR* szFontName   �t�H���g���i�ȗ��j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------ 
void CFontTexture::Draw(float posX, float posY, const TCHAR* szText, int fontsize, DWORD colorABGR, float fAlpha,
                        const TCHAR* szFontName)
{
    MATRIX4X4 mWorld;
    mWorld = XMMatrixTranslation(posX, posY, 0.0f);

    Draw(mWorld, szText, fontsize, colorABGR, fAlpha, szFontName);
}

//------------------------------------------------------------------------
//
//	�e�L�X�g�̕`��
//
//	�E���[���h�}�g���b�N�X�̏������̏ꍇ
//
//  MATRIX4X4 mWorld  �\���ʒu�@����@���[���h�}�g���b�N�X
//  const TCHAR* szText       �\������e�L�X�g
//  int fontsize       �t�H���g�T�C�Y
//  DWORD colorABGR    �����F�@colorABGR�́AABGR�̎w��B��F���F��(0x00ffffff)
//  float fAlpha       �����x�i�ȗ��j
//  const TCHAR* szFontName   �t�H���g���i�ȗ��j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------ 
void CFontTexture::Draw(MATRIX4X4 mWorld, const TCHAR* szText, int fontsize, DWORD colorABGR, float fAlpha,
                        const TCHAR* szFontName)
{
    // �����񂪂Ȃ��Ƃ��͕`�悵�Ȃ�
    if (szText[0] == _T('\0')) return; // -- 2017.11.22

    //
    // �O��܂ł́u�������F�A�T�C�Y�v���P��ʕ��E��������m_TextData�z��ɓ����Ă���̂�
    // �쐬�f�[�^������Ɠ������ǂ����`�F�b�N����
    //
    // �@�@����̏ꍇ�́A���łɍ쐬�ς݂̃t�H���g�e�N�X�`���[���g�p����
    // �A�@�����񂪈قȂ�ꍇ�̂݁A�t�H���g�e�N�X�`���[�̐������s��
    // �@�@�i�t�H���g�e�N�X�`���[�̐��������͔��ɏd�������Ȃ̂Łj

    if (m_TextData[m_Idx].m_szText == nullptr || _tcscmp(m_TextData[m_Idx].m_szText, szText) != 0 ||
        m_TextData[m_Idx].m_iFontsize != fontsize || m_TextData[m_Idx].m_dwColor != colorABGR || m_TextData[m_Idx].
        m_fAlpha != fAlpha) // �쐬�ς݂̕�����Ɠ��ꂩ�H
    {
        // �����񓙂ɕύX�̂������Ƃ�
        SAFE_DELETE_ARRAY(m_TextData[m_Idx].m_szText);
        m_TextData[m_Idx].m_szText = new TCHAR[_tcslen(szText) + 1]; // TCHAR�́A�P�������Q�o�C�g  // -- 2018.12.28
        m_TextData[m_Idx].m_dwKbn = 0; // 2D�t�H���g
        _tcscpy_s(m_TextData[m_Idx].m_szText, _tcslen(szText) + 1, szText);
        // ������̕ۑ�                           // -- 2018.12.28
        m_TextData[m_Idx].m_iFontsize = fontsize; // �t�H���g�T�C�Y��ۑ�
        m_TextData[m_Idx].m_dwColor = colorABGR; // �F��ۑ�
        m_TextData[m_Idx].m_fAlpha = fAlpha; // �����x��ۑ�

        // �t�H���g�e�N�X�`���[�̍쐬(2D�t�H���g�쐬)            // -- 2018.8.10
        CreateTex(0, 0, 0, szText, fontsize, colorABGR, szFontName);
    }

    // �o���オ�����e�N�X�`�����g���ĕ`����s��

    //�g�p����V�F�[�_�[�̃Z�b�g
    SetShader();

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_TextData[m_Idx].m_pVertexBufferFont, &stride, &offset);

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        //���[���h�s���n��
        cb.mW = XMMatrixTranspose(mWorld);

        //�r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
        cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
        cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
        cb.vUVOffset.x = 0;
        cb.vUVOffset.y = 0;
        cb.vColor = VECTOR4(1, 1, 1, 1); // -- 2020.1.24
        cb.vColor.w = m_TextData[m_Idx].m_fAlpha;
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // �e�N�X�`���L��
        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    //�e�N�X�`���[���V�F�[�_�[�ɓn��
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_TextData[m_Idx].m_pResourceView);

    //�v���~�e�B�u�������_�����O
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    //	�V�F�[�_�[�������Z�b�g
    ResetShader();

    // �ۑ�����TextData�z��̓Y�����P���₷�B
    m_Idx++;
    if (m_Idx >= TEXT_DATA_MAX)
    {
        MessageBox(
            0, _T(
                "�쐬����t�H���g�e�N�X�`���̍ő吔�𒴂��܂����B\nReflesh()�������Ă��܂����B\n�܂���TEXT_DATA_MAX�𑝂₵�Ă��������B"),
            _T(""), MB_OK);
        m_Idx--;
    }
}


//------------------------------------------------------------------------
//
//	�`��O�ɃV�F�[�_�[�����Z�b�g
//
//  �����@�@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CFontTexture::SetShader()
{
    //�g�p����V�F�[�_�[�̃Z�b�g
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);


    //���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    //���_�C���v�b�g���C�A�E�g���Z�b�g
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);

    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // �T���v���[���Z�b�g
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

    // ���ߐF�̃u�����f�B���O��ݒ�
    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    //Z�o�b�t�@�𖳌���
    m_pD3D->SetZBuffer(false); // -- 2019.4.19
}

//------------------------------------------------------------------------
//
//	�`���ɃV�F�[�_�[�������Z�b�g
//
//  �����@�@�Ȃ�
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------
void CFontTexture::ResetShader()
{
    //Z�o�b�t�@��L����
    m_pD3D->SetZBuffer(true); // -- 2019.4.19


    // �ʏ�̃u�����f�B���O�ɖ߂�
    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
}

//------------------------------------------------------------------------
//
//	�t�H���g�e�N�X�`���[�̍쐬
//
//	�EGDI����t�H���g�̃O���t�B�b�N�X�𓾂āA�e�N�X�`���[���쐬����
//	�@�쐬�����e�N�X�`���[�́Am_TextData[m_Idx]�z��Ɋi�[����
//
//  const DWORD&  dwKbn        �敪 0:�Q�c�t�H���g�@1:�R�c�t�H���g  // -- 2018.8.10
//  const float&  fDestWidth  �R�c�t�H���g  �\����                 // -- 2018.8.10
//  const float&  fDestHeight �R�c�t�H���g  �\������               // -- 2018.8.10
//  const TCHAR* text         �\������e�L�X�g
//  const int& fontsize       �t�H���g�T�C�Y
//  const DWORD& colorABGR    �����F�@colorABGR�́AABGR�̎w��B��F���F��(0x00ffffff)
//  const TCHAR* szFontName   �t�H���g���inullptr�ł��j
//
//	�߂�l  �Ȃ�
//
//------------------------------------------------------------------------ 
void CFontTexture::CreateTex(const DWORD& dwKbn, const float& fDestWidth, const float& fDestHeight, const TCHAR* text,
                             const int& fontsize, const DWORD& colorABGR, const TCHAR* fontname)
{
    // �t�H���g�̐���
    LOGFONT lf = {
        fontsize,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        SHIFTJIS_CHARSET,
        OUT_TT_ONLY_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        FIXED_PITCH | FF_MODERN,
        //TEXT("�c�e������")
        TEXT("�l�r �o�S�V�b�N")
        //TEXT("�l�r �S�V�b�N")
        //TEXT("HGP�n�p�p�߯�ߑ�")
        //TEXT("HGP�n�p�p�޼��UB")
    };
    if (fontname != nullptr)
        _tcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(TCHAR), fontname); // �t�H���g����ݒ�

    HFONT hFont = CreateFontIndirect(&lf);
    if (!(hFont))
    {
        MessageBox(0, _T("Sprite.cpp �e�N�X�`���p�̃t�H���g���쐬�ł��܂���"), _T(""), MB_OK);
    }


    // ������O���t�B�b�N�̐���

    DWORD dwTextlen = (DWORD)_tcslen(text); // �������i�o�C�g���ł͂Ȃ��j  // -- 2018.12.28
    DWORD dwTextHeight = 0;
    DWORD dwTextWidth = 0;
    DWORD dwAllWidth = 0;
    DWORD dwAllWidth2 = 0; // -- 2022.11.8
    FontData* pFontData = new FontData[dwTextlen];

    // �f�o�C�X�R���e�L�X�g�擾
    // �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
    HDC hdc = GetDC(nullptr);
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);


    for (DWORD i = 0; i < dwTextlen; i++)
    {
        // �����R�[�h�擾
        UINT code = 0;

#if _UNICODE
        // unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ�
        code = (UINT)*(text + i);
#else
        // �}���`�o�C�g�����̏ꍇ�A
        // 1�o�C�g�����̃R�[�h�̏ꍇ��1�o�C�g�ڂ�UINT�ϊ��A
        // 2�o�C�g�����̃R�[�h�̏ꍇ��[�擱�R�[�h]*256 + [�����R�[�h]
        BYTE* c = (BYTE*)(text + i);
        if (IsDBCSLeadByte(*c)) // 2�o�C�g�����̐擱�R�[�h���ǂ���
        {
            code = (BYTE)c[0] << 8 | (BYTE)c[1];
            i++;
        }
        else
        {
            code = c[0];
        }
#endif

        // �t�H���g�r�b�g�}�b�v�擾
        GetTextMetrics(hdc, &(pFontData + i)->TM);
        CONST MAT2 Mat = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
        DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &(pFontData + i)->GM, 0, nullptr, &Mat);
        // �o�b�t�@�T�C�Y�����Ԃ��Ă��炤
        if (size == 0) // �t�H���g�f�[�^���邩  2017.4.15
        {
            (pFontData + i)->ptr = nullptr; // �t�H���g�f�[�^�������Ƃ�  2017.4.15
        }
        else
        {
            (pFontData + i)->ptr = new BYTE[size];
            GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &(pFontData + i)->GM, size, (pFontData + i)->ptr, &Mat);
        }
        if ((int)dwTextWidth < (pFontData + i)->GM.gmCellIncX) dwTextWidth = (pFontData + i)->GM.gmCellIncX;
        // �P�����̕�
        if ((int)dwTextHeight < (pFontData + i)->TM.tmHeight) dwTextHeight = (pFontData + i)->TM.tmHeight;
        // �P�����̍���
        if ((int)dwTextHeight < (pFontData + i)->GM.gmBlackBoxY) dwTextHeight = (pFontData + i)->GM.gmBlackBoxY;
        // �P�����̍����ƃt�H���g�r�b�g�}�b�v�̍����̑傫����    // -- 2022.11.8
        dwAllWidth += (pFontData + i)->GM.gmCellIncX; // ������S�̂̒����i�h�b�g���j
        dwAllWidth2 += (pFontData + i)->GM.gmBlackBoxX + (4 - ((pFontData + i)->GM.gmBlackBoxX % 4)) % 4;
        // �t�H���g�r�b�g�}�b�v�̕�  // -- 2022.11.8
    }

    if (dwAllWidth < dwAllWidth2)
    // ������S�̂̒����i�h�b�g���j�ƃt�H���g�r�b�g�}�b�v�̕��̑傫�����Ńe�N�X�`�������  // -- 2022.11.8
    {
        dwAllWidth = dwAllWidth2;
    }

    // �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    ReleaseDC(nullptr, hdc);

    // ----------------------------------------------------
    // ������t�H���g�r�b�g�}�b�v�̑傫���Ńo�[�e�b�N�X�o�b�t�@�[�쐬
    if (dwKbn == 0) // -- 2018.8.10
    {
        CreateVB(dwAllWidth, dwTextHeight); // 2D�t�H���g
    }
    else
    {
        CreateVB3D(fDestWidth, fDestHeight); // 3D�t�H���g
    }

    // ----------------------------------------------------
    // �������݉\�e�N�X�`���쐬
    // CPU�ŏ������݂��ł���e�N�X�`�����쐬

    ID3D11Texture2D* pTexture2D; // 2�c�e�N�X�`��

    D3D11_TEXTURE2D_DESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.Width = dwAllWidth; // ������P�s���̑傫��
    desc.Height = dwTextHeight; // �����̍����ő�l
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA(255,255,255,255)�^�C�v
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC; // ���I�i�������݂��邽�߂̕K�{�����j
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // �V�F�[�_���\�[�X�Ƃ��Ďg��
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU����A�N�Z�X���ď�������OK

    if (FAILED(m_pD3D->m_pDevice->CreateTexture2D(&desc, 0, &pTexture2D)))
    {
        MessageBox(0, _T("Sprite.cpp �e�N�X�`���p�̃t�H���g���쐬�ł��܂���"), _T(""), MB_OK);
    }


    D3D11_MAPPED_SUBRESOURCE hMappedResource;

    if (FAILED(m_pD3D->m_pDeviceContext->Map(
        pTexture2D,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &hMappedResource)))
    {
        MessageBox(0, _T("Sprite.cpp �e�N�X�`���}�b�v  �t�H���g  ���s"), _T(""), MB_OK);
    }


    // �쐬�����e�N�X�`���i�r���[�j�Ƀt�H���g������������
    BYTE* pBits = (BYTE*)hMappedResource.pData;
    ZeroMemory(pBits, hMappedResource.RowPitch * dwTextHeight);

    DWORD dwAllWidthWk = 0;
    for (DWORD i = 0; i < dwTextlen; i++)
    {
        // �t�H���g���̏�������
        // iOfs_x, iOfs_y : �����o���ʒu(����)
        // iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
        // Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
        int iOfs_x = (pFontData + i)->GM.gmptGlyphOrigin.x;
        int iOfs_y = (pFontData + i)->TM.tmAscent - (pFontData + i)->GM.gmptGlyphOrigin.y;
        int iBmp_w = (pFontData + i)->GM.gmBlackBoxX + (4 - ((pFontData + i)->GM.gmBlackBoxX % 4)) % 4;
        int iBmp_h = (pFontData + i)->GM.gmBlackBoxY;
        if (iOfs_x < 0) iOfs_x = 0; // -- 2022.11.8
        if (iOfs_y < 0) iOfs_y = 0; // -- 2022.11.8
        int Level = 17;
        int x, y;
        DWORD Alpha, Color;

        for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
        {
            for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
            {
                if ((pFontData + i)->ptr == nullptr) // �t�H���g�f�[�^�������Ƃ�  2017.4.15
                {
                    Alpha = 0; // �����F�Ƃ���
                }
                else
                {
                    Alpha = (255 * (pFontData + i)->ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);
                }
                Color = colorABGR | (Alpha << 24);

                memcpy((BYTE*)pBits + hMappedResource.RowPitch * y + 4 * (x + dwAllWidthWk), &Color, sizeof(DWORD));
            }
        }
        dwAllWidthWk += (pFontData + i)->GM.gmCellIncX; // �P�������i�߂�
    }

    m_pD3D->m_pDeviceContext->Unmap(pTexture2D, 0);


    // �e�N�X�`�������擾����
    D3D11_TEXTURE2D_DESC texDesc;
    pTexture2D->GetDesc(&texDesc);

    // ShaderResourceView�̏����쐬����
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

    SAFE_RELEASE(m_TextData[m_Idx].m_pResourceView); // �Q��ڈȍ~�̍쐬�ɑΉ�

    // �V�F�[�_�[���\�[�X�r���[�̍쐬
    if (FAILED(m_pD3D->m_pDevice->CreateShaderResourceView(pTexture2D, &srvDesc, &m_TextData[m_Idx].m_pResourceView)))
    {
        MessageBox(0, _T("Sprite.cpp �t�H���g�p�@ShaderResourceView�쐬�Ɏ��s���܂���"), nullptr, MB_OK);
    }

    // �쐬�����ꎞ���\�[�X�̉��
    SAFE_RELEASE(pTexture2D);

    for (DWORD i = 0; i < dwTextlen; i++)
    {
        SAFE_DELETE_ARRAY((pFontData + i)->ptr);
    }
    SAFE_DELETE_ARRAY(pFontData);
}


//------------------------------------------------------------------------ // -- 2018.8.10
//
//	�R�c�i�r���{�[�h�j�t�H���g�̃o�[�e�B�N�X�o�b�t�@�쐬	
//
//  const float& fDestWidth         �\����
//  const float& fDestHeight        �\������
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CFontTexture::CreateVB3D(const float& fDestWidth, const float& fDestHeight)
{
    // �r���{�[�h�e�N�X�`���p�̔|���S�����쐬���A�o�[�e�b�N�X�o�b�t�@���쐬����
    m_fDestWidth = fDestWidth;
    m_fDestHeight = fDestHeight;

    //�o�[�e�b�N�X�o�b�t�@�[�쐬(�������̍��W�ō쐬)
    SpriteVertex vertices[] =
    {
        VECTOR3(-m_fDestWidth / 2, m_fDestHeight / 2, 0), VECTOR2(1, 0), // ���_1  ����
        VECTOR3(-m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2(1, 1), // ���_2�@����    // -- 2024.3.23
        VECTOR3(m_fDestWidth / 2, m_fDestHeight / 2, 0), VECTOR2(0, 0), // ���_3�@�E��    // -- 2024.3.231
        VECTOR3(m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2(0, 1), // ���_4�@�E��
    };

    // �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
    if (m_TextData[m_Idx].m_pVertexBufferFont == nullptr)
    {
        // �V�K�쐬����
        D3D11_BUFFER_DESC bd;
        //bd.Usage = D3D11_USAGE_DEFAULT;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bd.CPUAccessFlags = 0;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_TextData[m_Idx].m_pVertexBufferFont)))
        {
            MessageBox(0, _T("Sprite.cpp �o�[�e�b�N�X�o�b�t�@�[3D�쐬���s"), nullptr, MB_OK);
            return E_FAIL;
        }
    }
    else
    {
        // ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_TextData[m_Idx].m_pVertexBufferFont, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4); // 4���_���R�s�[
            m_pD3D->m_pDeviceContext->Unmap(m_TextData[m_Idx].m_pVertexBufferFont, 0);
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------ // -- 2024.3.23
//
//	�R�c�i�r���{�[�h�j�t�H���g�I�u�W�F�N�g����ʂɃ����_�����O	
//
//------------------------------------------------------------------------ 
bool CFontTexture::Draw3D(const VECTOR3& vPos, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR,
                          const float& fAlpha, const TCHAR* szFontName)
{
    return Draw3D(vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, szText, vSize, colorABGR,
                  fAlpha, szFontName);
}

//------------------------------------------------------------------------ // -- 2018.8.10
//
//	�R�c�i�r���{�[�h�j�t�H���g�I�u�W�F�N�g����ʂɃ����_�����O	
//
//	����
//		const VECTOR3& vPos     �\���ʒu
//		const MATRIX4X4& mView  �r���[�}�g���b�N�X
//		const MATRIX4X4& mProj  �v���W�F�N�V�����}�g���b�N�X
//		const VECTOR3& vEye     ���_�ʒu
//		const TCHAR* szText     �\������e�L�X�g
//		const VECTOR2& vSize    �\���T�C�Y
//		const DWORD& colorABGR   �����F�@colorABGR�́AABGR�̎w��B��F���F��(0x00ffffff)
//		const float& fAlpha      �����x�i�ȗ��j
//		const TCHAR* szFontNam  �t�H���g���i�ȗ��j
//
//	�߂�l bool
//		true      �\���p��
//		false     �\���I��
//
//------------------------------------------------------------------------
bool CFontTexture::Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye,
                          const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha,
                          const TCHAR* szFontName)
{
    if (szText[0] == _T('\0')) return false; // �����񂪂Ȃ��Ƃ��͕`�悵�Ȃ�

    //
    // �O��܂ł́u�������F�A�T�C�Y�v���P��ʕ��E��������m_TextData�z��ɓ����Ă���̂�
    // �쐬�f�[�^������Ɠ������ǂ����`�F�b�N����
    //
    // �@�@����̏ꍇ�́A���łɍ쐬�ς݂̃t�H���g�e�N�X�`���[���g�p����
    // �A�@�����񂪈قȂ�ꍇ�̂݁A�t�H���g�e�N�X�`���[�̐������s��
    // �@�@�i�t�H���g�e�N�X�`���[�̐��������͔��ɏd�������Ȃ̂Łj

    DWORD fontsize = (DWORD)vSize.y * 100;

    if (m_TextData[m_Idx].m_dwKbn != 1 || m_TextData[m_Idx].m_szText == nullptr || _tcscmp(
            m_TextData[m_Idx].m_szText, szText) != 0 ||
        m_TextData[m_Idx].m_iFontsize != fontsize || m_TextData[m_Idx].m_dwColor != colorABGR || m_TextData[m_Idx].
        m_fAlpha != fAlpha) // �쐬�ς݂̕�����Ɠ��ꂩ�H
    {
        // �����񓙂ɕύX�̂������Ƃ�
        SAFE_DELETE_ARRAY(m_TextData[m_Idx].m_szText);
        m_TextData[m_Idx].m_szText = new TCHAR[_tcslen(szText) + 1]; // TCHAR�́A�P�������Q�o�C�g  // -- 2018.12.28
        m_TextData[m_Idx].m_dwKbn = 1; // 3D�t�H���g
        _tcscpy_s(m_TextData[m_Idx].m_szText, _tcslen(szText) + 1, szText); // ������̕ۑ�
        m_TextData[m_Idx].m_iFontsize = fontsize; // �t�H���g�T�C�Y��ۑ�
        m_TextData[m_Idx].m_dwColor = colorABGR; // �F��ۑ�
        m_TextData[m_Idx].m_fAlpha = fAlpha; // �����x��ۑ�

        // �t�H���g�e�N�X�`���[�̍쐬(3D�t�H���g�쐬)
        CreateTex(1, vSize.x, vSize.y, szText, fontsize, colorABGR, szFontName);
    }

    // �o���オ�����e�N�X�`�����g���ĕ`����s��

    //�r���{�[�h�́A���_���������[���h�g�����X�t�H�[�������߂�
    MATRIX4X4 mWorld = GetLookatMatrix(vPos, vEye);

    //�g�p����V�F�[�_�[�̃Z�b�g
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_TextData[m_Idx].m_pVertexBufferFont, &stride, &offset);

    // �����F�̃u�����f�B���O��ݒ�
    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    ZeroMemory(&cb, sizeof(cb));

    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        //���[���h�A�J�����A�ˉe�s��A�e�N�X�`���[�I�t�Z�b�g��n��
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        cb.vUVOffset.x = 0;
        cb.vUVOffset.y = 0;
        cb.vColor = VECTOR4(1, 1, 1, 1); // -- 2020.1.24
        cb.vColor.w = fAlpha;
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // �e�N�X�`���L��

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }
    //���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    //���_�C���v�b�g���C�A�E�g���Z�b�g
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);
    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    //�e�N�X�`���[���V�F�[�_�[�ɓn��
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_TextData[m_Idx].m_pResourceView);
    //�v���~�e�B�u�������_�����O
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // �ʏ�̃u�����f�B���O�ɖ߂�
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    // �ۑ�����TextData�z��̓Y�����P���₷�B
    m_Idx++;
    if (m_Idx >= TEXT_DATA_MAX)
    {
        MessageBox(
            0, _T(
                "�쐬����t�H���g�e�N�X�`���̍ő吔�𒴂��܂����B\nReflesh()�������Ă��܂����B\n�܂���TEXT_DATA_MAX�𑝂₵�Ă��������B"),
            _T(""), MB_OK);
        m_Idx--;
    }

    return true;
}
