// ========================================================================================
//
// 3Dおよび2Dスプライト描画ライブラリ                                ver 3.3        2024.10.5
//
//   Sprite3D.cpp Direct3D.h などと連携して動作します
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
//  スプライトイメージのコンストラクタ  
//
//  引数：CShader* pShader
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
//  スプライトイメージのコンストラクタ  
//
//  CShader* pShader
//  const TCHAR* TName  スプライトファイル名
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
//  スプライトイメージのデストラクタ    
//
//------------------------------------------------------------------------
CSpriteImage::~CSpriteImage()
{
    SAFE_RELEASE(m_pTexture);
}

//------------------------------------------------------------------------
//  スプライトイメージの読み込み  
//
//  指定されたスプライトファイルからイメージを読み込む
//
//  const TCHAR* TName  スプライトファイル名
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = ファイルが見つからない等の失敗
//------------------------------------------------------------------------
HRESULT CSpriteImage::Load(const TCHAR* TName)
{
    if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(TName, &m_pTexture, m_dwImageWidth, m_dwImageHeight)))
    {
        std::string name = TName;
        std::string message =
            std::string("SpriteLoad:false ") + name;
        MessageBox(0, message.c_str(), TName, MB_OK
        );
        return E_FAIL;
    }
    return S_OK;
}

CSprite::CSprite() : CSprite(GameDevice()->m_pShader)
{
}

//------------------------------------------------------------------------
//
//  スプライトのコンストラクタ  
//
//  引数：CShader* pShader
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
//  スプライトのコンストラクタ  
//
//  引数
//  CSpriteImage* pImage      スプライトイメージポインタ
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
//  スプライトのコンストラクタ  
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//  const DWORD&         srcX        切り取り元の開始位置X座標
//  const DWORD&         srcY        切り取り元の開始位置Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
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
//  スプライトのコンストラクタ  
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//  const DWORD&         srcX        切り取り元の開始位置X座標
//  const DWORD&         srcY        切り取り元の開始位置Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//  const DWORD&         destwidth   表示幅
//  const DWORD&         destheight  表示高さ
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
//  スプライトのデストラクタ    
//
//------------------------------------------------------------------------
CSprite::~CSprite()
{
    SAFE_RELEASE(m_pVertexBufferSprite);
    SAFE_RELEASE(m_pVertexBufferLine);
    SAFE_RELEASE(m_pVertexBufferRect);
    SAFE_RELEASE(m_pVertexBufferBillSprite); // 3DSprite用
}

//------------------------------------------------------------------------
//                                                         // -- 2017.10.9
//  スプライトのイメージを設定  
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//
//------------------------------------------------------------------------
void CSprite::SetImage(CSpriteImage* pImage)
{
    m_pImage = pImage;
}

//------------------------------------------------------------------------
//
//  スプライトのイメージ位置、サイズ情報を更新する 
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//  const DWORD&         srcX        切り取り元の開始位置X座標
//  const DWORD&         srcY        切り取り元の開始位置Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 失敗
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
//  スプライトのイメージ位置、サイズ情報を更新する 
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//  const DWORD&         srcX        切り取り元の開始位置X座標
//  const DWORD&         srcY        切り取り元の開始位置Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//  const DWORD&         destwidth   表示幅
//  const DWORD&         destheight  表示高さ
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 失敗
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
//  スプライトのイメージ位置、サイズ情報を更新する 
//
//  const DWORD&         srcX        切り取り元の開始位置X座標
//  const DWORD&         srcY        切り取り元の開始位置Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 失敗
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight)
{
    return SetSrc(srcX, srcY, srcwidth, srcheight, srcwidth, srcheight);
}

//------------------------------------------------------------------------
//
//  スプライトのイメージ位置、サイズ情報を更新する 
//
//  const DWORD&         srcX        切り取り元の開始位置X座標
//  const DWORD&         srcY        切り取り元の開始位置Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//  const DWORD&         destwidth   表示幅
//  const DWORD&         destheight  表示高さ
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 失敗
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc(const DWORD& srcX, const DWORD& srcY, const DWORD& srcwidth, const DWORD& srcheight,
                        const DWORD& destwidth, const DWORD& destheight)
{
    // スプライト用のポリゴンを作成し、頂点バッファを更新する
    m_dwSrcX = srcX;
    m_dwSrcY = srcY;
    m_dwSrcWidth = srcwidth;
    m_dwSrcHeight = srcheight;
    m_dwDestWidth = destwidth;
    m_dwDestHeight = destheight;

    // 頂点バッファデータ
    // インデックス構成：z値は1.0で固定（プロジェクション変換後にz=1になるよう調整）。描画順に注意。
    SpriteVertex vertices[] =
    {
        {
            VECTOR3(0, (float)m_dwDestHeight, 0),
            VECTOR2((float)m_dwSrcX / m_pImage->m_dwImageWidth,
                    (float)(m_dwSrcY + m_dwSrcHeight) / m_pImage->m_dwImageHeight)
        }, // 頂点1：左下
        {
            VECTOR3(0, 0, 0),
            VECTOR2((float)m_dwSrcX / m_pImage->m_dwImageWidth, (float)m_dwSrcY / m_pImage->m_dwImageHeight)
        }, // 頂点2：左上
        {
            VECTOR3((float)m_dwDestWidth, (float)m_dwDestHeight, 0),
            VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_pImage->m_dwImageWidth,
                    (float)(m_dwSrcY + m_dwSrcHeight) / m_pImage->m_dwImageHeight)
        }, // 頂点3：右下
        {
            VECTOR3((float)m_dwDestWidth, 0, 0),
            VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_pImage->m_dwImageWidth,
                    (float)m_dwSrcY / m_pImage->m_dwImageHeight)
        }, // 頂点4：右上
    };

    // 頂点バッファが未作成か、あるいは更新が必要かをチェック
    if (m_pVertexBufferSprite == nullptr)
    {
        // 新規作成
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferSprite)))
        {
            MessageBox(0, _T("Sprite.cpp 頂点バッファ作成失敗"), nullptr, MB_OK);
            return E_FAIL;
        }
    }
    else
    {
        // 作成済みの場合は、内容のみ更新（動的な書き換え）
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4); // 4頂点分コピー
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferSprite, 0);
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------
//
//  スプライトを指定位置にレンダリング
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//  const float&         posX        表示位置X座標
//  const float&         posY        表示位置Y座標
//  const DWORD&         srcX        切り取り元X座標
//  const DWORD&         srcY        切り取り元Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//  const float&         fAlpha      透明度
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
//  const DWORD&         srcX        切り取り元のX座標
//  const DWORD&         srcY        切り取り元のY座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//  const DWORD&         destwidth   表示幅
//  const DWORD&         destheight  表示高さ
//  const float&         fAlpha      透明度
//
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
//  スプライトを指定位置にレンダリング
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//  const MATRIX4X4&    mWorld      表示位置用ワールドマトリックス
//  const DWORD&         srcX        切り取り元X座標
//  const DWORD&         srcY        切り取り元Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//  const float&         fAlpha      透明度
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
//  スプライトを指定位置にレンダリング
//
//  CSpriteImage* pImage      スプライトイメージポインタ
//  const MATRIX4X4&    mWorld      表示位置用ワールドマトリックス
//  const DWORD&         srcX        切り取り元X座標
//  const DWORD&         srcY        切り取り元Y座標
//  const DWORD&         srcwidth    切り取り元の幅
//  const DWORD&         srcheight   切り取り元の高さ
//  const DWORD&         destwidth   表示幅
//  const DWORD&         destheight  表示高さ
//  const float&         fAlpha      透明度
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
//  スプライトを指定位置にレンダリング
//
//  const float&         posX     表示位置X座標
//  const float&         posY     表示位置Y座標
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
//  スプライトを指定位置にレンダリング（サブルーチン）
//
//  const MATRIX4X4&    mWorld   表示位置用ワールドマトリックス
//
//------------------------------------------------------------------------
void CSprite::Draw(const MATRIX4X4& mWorld)
{
    // 使用するシェーダーをセット
    SetShader();

    // 頂点バッファをセット
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferSprite, &stride, &offset);

    // シェーダーの定数バッファへ各種データを渡す
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        // ワールド行列を渡す
        cb.mW = XMMatrixTranspose(mWorld);

        // ビューポートサイズを渡す（スクリーン座標計算用）
        cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
        cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
        cb.vUVOffset.x = (float)m_ofX / m_pImage->m_dwImageWidth;
        cb.vUVOffset.y = (float)m_ofY / m_pImage->m_dwImageHeight;
        cb.vColor = m_vDiffuse; // -- 2020.1.24
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // テクスチャあり
        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    // テクスチャをシェーダーへ渡す
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);

    // プリミティブをレンダリング（4頂点のトライアングルストリップ）
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // シェーダー設定をリセット
    ResetShader();
}

//------------------------------------------------------------------------
//
//  ラインを指定位置にレンダリング（サブルーチン）                                         // -- 2017.10.9
//
//  （注意：色と透明度は m_vDiffuse を使用せず、引数で指定されたものを使用）
//
//  const float& StartX     ライン描画の開始X座標
//  const float& StartY     ライン描画の開始Y座標
//  const float& EndX       ライン描画の終了X座標
//  const float& EndY       ライン描画の終了Y座標
//  const DWORD& WidthIn    ラインの太さ（ピクセル）
//  const DWORD& colorABGR  ライン色（0xAABBGR形式）
//  const float& fAlpha     透明度（初期値1.0f）
//
//------------------------------------------------------------------------
void CSprite::DrawLine(const float& StartX, const float& StartY, const float& EndX, const float& EndY,
                       const DWORD& WidthIn, const DWORD& colorABGR, const float& fAlpha)
{
    DWORD Width = WidthIn; // ラインの太さ

    // 画面外に完全に外れている場合は描画処理を行わない（クリッピング）
    if ((StartX < 0 && EndX < 0) || (StartY < 0 && EndY < 0) ||
        (StartX > m_pD3D->m_dwWindowWidth && EndX > m_pD3D->m_dwWindowWidth) ||
        (StartY > m_pD3D->m_dwWindowHeight && EndY > m_pD3D->m_dwWindowHeight))
    {
        return;
    }

    // 使用するシェーダーをセット
    SetShader();

    // ライン用の頂点バッファデータ
    SpriteVertex vertices[] =
    {
        {VECTOR3(StartX, StartY, 0), VECTOR2(0, 0)}, // 頂点1
        {VECTOR3(EndX, EndY, 0), VECTOR2(0, 0)} // 頂点2
    };

    // 頂点バッファが未作成かチェックし、動的に更新
    if (m_pVertexBufferLine == nullptr)
    {
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 2;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferLine)))
        {
            MessageBox(0, _T("Sprite.cpp 頂点バッファ作成失敗(LINE)"), nullptr, MB_OK);
            return;
        }
    }
    else
    {
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferLine, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 2);
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferLine, 0);
        }
    }

    // 色データの変換
    VECTOR4 color;
    color.x = ((colorABGR & 0x000000ff) >> 0) / 255.0f; // R
    color.y = ((colorABGR & 0x0000ff00) >> 8) / 255.0f; // G
    color.z = ((colorABGR & 0x00ff0000) >> 16) / 255.0f; // B
    color.w = fAlpha; // A

    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferLine, &stride, &offset);

    // プリミティブトポロジーをラインリストに変更
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    // ライン描画時はテクスチャを使用しない
    ID3D11ShaderResourceView* Nothing[1] = {0};
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

    MATRIX4X4 mWorld;

    // --- ラインの太さを出すための計算 ---
    // ラインに垂直なベクトル（法線）を求め、位置をずらしながら重ねて描画する
    VECTOR2 vNrm, vDif = VECTOR2(EndX - StartX, EndY - StartY), vLen;
    vNrm.x = vDif.y;
    vNrm.y = vDif.x * -1; // 垂直ベクトルを作成

    XMVECTOR vNrmVec = XMLoadFloat2(&vNrm);
    XMVECTOR vLenVec = XMVector2Length(vNrmVec);
    XMStoreFloat2(&vLen, vLenVec);

    if (vLen.x > 0.0f)
    {
        vNrm.x /= vLen.x;
        vNrm.y /= vLen.x;
    }

    if (Width < 1) Width = 1;

    // 太さの分だけずらしてループ描画
    for (DWORD i = 0; i < Width; i++)
    {
        mWorld = XMMatrixIdentity();

        // 交互に法線方向へずらす
        float offsetScale = 0.8f * i * 0.5f;
        if (i % 2 == 0)
        {
            mWorld._41 = vNrm.x * offsetScale;
            mWorld._42 = vNrm.y * offsetScale;
        }
        else
        {
            mWorld._41 = -vNrm.x * offsetScale;
            mWorld._42 = -vNrm.y * offsetScale;
        }

        D3D11_MAPPED_SUBRESOURCE pData;
        CONSTANT_BUFFER_SPRITE cb;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
        {
            cb.mW = XMMatrixTranspose(mWorld);
            cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
            cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
            cb.vUVOffset = VECTOR2(0, 0);
            cb.vColor = color;
            cb.vMatInfo = VECTOR4(0, 0, 0, 0); // テクスチャなしフラグ
            memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
            m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
        }
        m_pD3D->m_pDeviceContext->Draw(2, 0);
    }

    // トポロジーを元に戻す
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    ResetShader();
}

//------------------------------------------------------------------------
//                                                         // -- 2018.3.20
//  矩形のレンダリング（サブルーチン）
//
//  const float& posX       矩形表示位置X
//  const float& posY       矩形表示位置Y
//  const DWORD& width      矩形幅
//  const DWORD& height     矩形高さ
//  const DWORD& colorABGR  描画色
//  const float& fAlpha     透明度
//
//------------------------------------------------------------------------
void CSprite::DrawRect(const float& posX, const float& posY, const DWORD& width, const DWORD& height,
                       const DWORD& colorABGR, const float& fAlpha)
{
    SetShader();

    SpriteVertex vertices[] =
    {
        {VECTOR3(0, (float)height, 0), VECTOR2(0, 0)}, // 左下
        {VECTOR3(0, 0, 0), VECTOR2(0, 0)}, // 左上
        {VECTOR3((float)width, (float)height, 0), VECTOR2(0, 0)}, // 右下
        {VECTOR3((float)width, 0, 0), VECTOR2(0, 0)}, // 右上
    };

    if (m_pVertexBufferRect == nullptr)
    {
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferRect)))
        {
            MessageBox(0, _T("Sprite.cpp 頂点バッファ作成失敗(RECT)"), nullptr, MB_OK);
            return;
        }
    }
    else
    {
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferRect, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4);
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferRect, 0);
        }
    }

    VECTOR4 color;
    color.x = ((colorABGR & 0x000000ff) >> 0) / 255.0f;
    color.y = ((colorABGR & 0x0000ff00) >> 8) / 255.0f;
    color.z = ((colorABGR & 0x00ff0000) >> 16) / 255.0f;
    color.w = fAlpha;

    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferRect, &stride, &offset);

    ID3D11ShaderResourceView* Nothing[1] = {0};
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        cb.mW = XMMatrixTranspose(XMMatrixTranslation(posX, posY, 0.0f));
        cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
        cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
        cb.vUVOffset = VECTOR2(0, 0);
        cb.vColor = color;
        cb.vMatInfo = VECTOR4(0, 0, 0, 0);
        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    ResetShader();
}

//------------------------------------------------------------------------
//  円形のスプライト描画（自作関数）
//
//  シェーダー側のピクセル描画で円形マスクをかける処理を想定
//------------------------------------------------------------------------
void CSprite::DrawCircle(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcWid,
                         DWORD srcHei, float startRad, float endRad, float fAlpha)
{
    m_pImage = pImage;

    // UV座標を0～1に固定（シェーダー側で円の計算に使用するため、テクスチャ全体を1つの円とみなす）
    SpriteVertex vertices[] =
    {
        {VECTOR3(0, (float)srcHei, 0), VECTOR2(0.0f, 1.0f)}, // 左下
        {VECTOR3(0, 0, 0), VECTOR2(0.0f, 0.0f)}, // 左上
        {VECTOR3((float)srcWid, (float)srcHei, 0), VECTOR2(1.0f, 1.0f)}, // 右下
        {VECTOR3((float)srcWid, 0, 0), VECTOR2(1.0f, 0.0f)}, // 右上
    };

    if (m_pVertexBufferSprite)
    {
        D3D11_MAPPED_SUBRESOURCE mapResource;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_pVertexBufferSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource)))
        {
            memcpy_s(mapResource.pData, sizeof(vertices), vertices, sizeof(vertices));
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferSprite, 0);
        }
    }

    SetShader();

    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    MATRIX4X4 mWorld = XMMatrixTranslation(posX, posY, 0.0f);
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferSprite, &stride, &offset);

    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        cb.mW = XMMatrixTranspose(mWorld);
        cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
        cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
        cb.vUVOffset = VECTOR2(0, 0);
        cb.vColor = VECTOR4(1, 1, 1, fAlpha);

        // --- 円形描画のための特殊情報 ---
        cb.vMatInfo.x = 1.0f; // 円形マスク有効フラグ
        cb.vMatInfo.y = startRad; // 扇形の開始角度
        cb.vMatInfo.z = endRad; // 扇形の終了角度
        cb.vMatInfo.w = 1.0f; // 円形モードON

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    ResetShader();
}

void CSprite::DrawArc(CSpriteImage* pImage, float posX, float posY, DWORD srcX, DWORD srcY, DWORD srcWid, DWORD srcHei,
                      ArcDrawParams& arcParams, float fAlpha)
{
    m_pImage = pImage;
    SetSrc(srcX, srcY, srcWid, srcHei);

    // 円マスクシェーダは UV を 0〜1 前提に扱うため、直前の描画に依存せず
    // ここで頂点 UV を 0〜1 に確定させる（これを省くと直前スプライトの UV が
    // 残り、円マスク計算がずれて描画されない）
    SpriteVertex vertices[] = {
        {VECTOR3(0, static_cast<float>(srcHei), 0), VECTOR2(0, 1)},
        {VECTOR3(0, 0, 0), VECTOR2(0, 0)},
        {VECTOR3(static_cast<float>(srcWid), static_cast<float>(srcHei), 0), VECTOR2(1, 1)},
        {VECTOR3(static_cast<float>(srcWid), 0, 0), VECTOR2(1, 0)},
    };

    if (m_pVertexBufferSprite)
    {
        D3D11_MAPPED_SUBRESOURCE mapResource;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_pVertexBufferSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource)))
        {
            memcpy_s(mapResource.pData, sizeof(vertices), vertices, sizeof(vertices));
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferSprite, 0);
        }
    }

    SetShader();

    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;

    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferSprite, &stride, &offset);

    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    //CPUからGPUのメモリを直接読み書きできるようにするための窓口を開く関数
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0,D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        //shader座標に変換
        cb.mW = XMMatrixTranspose(XMMatrixTranslation(posX, posY, 0.0f));
        //ウィンドウのサイズをシェーダーに渡す。
        cb.ViewPortWidth = static_cast<float>(m_pD3D->m_dwWindowWidth);
        cb.ViewPortHeight = static_cast<float>(m_pD3D->m_dwWindowHeight);
        //スクロール表示などに使うが今回は不要
        cb.vUVOffset = VECTOR2(0, 0);
        //色の乗算値(R)1,(G)1,(B)1は元の色のまま
        cb.vColor = VECTOR4(1, 1, 1, fAlpha);
        //x:テクスチャの有無, w:円形モードON/OFF
        cb.vMatInfo = VECTOR4(1, 0, 0, 1); //x = テクスチャあり, w = 円形モードON
        //構造体の内容をGPUに書き出し
        memcpy_s(pData.pData, pData.RowPitch, &cb, sizeof(cb));
        //CPUからGPUのメモリを直接読み書きできるようにするための窓口を閉じる関数
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    //Shaderのb1に書き込む内容の構造体
    CONSTANT_BUFFER_ARC arcCb;
    arcCb.startAngle = arcParams.startAngle * XM_PI / 180.0f;
    //孤の長さをラジアンで計算
    //arcParams.clockwiseがTrueのとき、時計回り
    arcCb.arcSpan = arcParams.ratio * XM_PI * 2.0f * (arcParams.clockwise ? 1.0f : -1.0f);
    //内半径(0~0.5)
    arcCb.innerRadius = arcParams.innerRadius;
    //UV範囲を設定
    arcCb.uvMin = VECTOR2((float)m_dwSrcX / m_pImage->m_dwImageWidth,
                          (float)m_dwSrcY / m_pImage->m_dwImageHeight);
    arcCb.uvMax = VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_pImage->m_dwImageWidth,
                          (float)(m_dwSrcY + m_dwSrcHeight) / m_pImage->m_dwImageHeight);
    //以前の内容を破棄して新しく書き込み
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferArc, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        memcpy_s(pData.pData, pData.RowPitch, &arcCb, sizeof(arcCb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferArc, 0);
    }
    //Shaderにb1として使えと指示
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferArc);
    //Shaderに画像を使うよう指示
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);
    //GPUが描画の実行、4：描画する頂点数、0：描画開始する頂点の先頭インデックス
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    ResetShader();
}

//------------------------------------------------------------------------
//
//  描画前にシェーダーをセット
//
//  引数：なし
//
//  戻り値：なし
//
//------------------------------------------------------------------------
void CSprite::SetShader()
{
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    // 各種シェーダーで使う定数バッファを登録
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);

    // 頂点インプットレイアウトをセット
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);

    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // サンプラー（線形補間）をセット
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

    // ブレンドステートの切り替え
    UINT mask = 0xffffffff;
    if (m_nBlend == 1)
    {
        // 1: アルファブレンド（半透明描画）
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
    }
    else if (m_nBlend == 2)
    {
        // 2: 加算合成（エフェクト等）
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, nullptr, mask);
    }

    // Zバッファ（深度テスト）を一時的にオフにする（2D描画の場合など）
    m_pD3D->SetZBuffer(false); // -- 2019.4.19
}

//------------------------------------------------------------------------
//
//  描画後にシェーダー設定をリセット
//
//  引数：なし
//
//  戻り値：なし
//
//------------------------------------------------------------------------
void CSprite::ResetShader()
{
    // Zバッファを有効に戻す
    m_pD3D->SetZBuffer(true); // -- 2019.4.19

    // ブレンドステートを通常に戻す
    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
}


//------------------------------------------------------------------------ // -- 2018.8.10
//
//  3Dビルボード（常にカメラを向く）スプライトの頂点バッファ設定
//
//  CSpriteImage* pImage      画像リソース
//  const float& fDestWidth   表示幅
//  const float& fDestHeight  表示高さ
//  const DWORD& dwSrcX       切り取り元座標X
//  const DWORD& dwSrcY       切り取り元座標Y
//  const DWORD& dwSrcWidth   切り取り幅
//  const DWORD& dwSrcHeight  切り取り高さ
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 失敗
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
//  3Dビルボードスプライトの頂点バッファ設定（実体）
//
//------------------------------------------------------------------------
HRESULT CSprite::SetSrc3D(const float& fDestWidth, const float& fDestHeight, const DWORD& dwSrcX, const DWORD& dwSrcY,
                          const DWORD& dwSrcWidth, const DWORD& dwSrcHeight)
{
    // ビルボード用のポリゴン計算（中心座標を基準に作成）
    m_dwSrcX = dwSrcX;
    m_dwSrcY = dwSrcY;
    m_dwSrcWidth = dwSrcWidth;
    m_dwSrcHeight = dwSrcHeight;
    m_fDestWidth = fDestWidth;
    m_fDestHeight = fDestHeight;

    DWORD dwImageWidth = m_pImage->m_dwImageWidth;
    DWORD dwImageHeight = m_pImage->m_dwImageHeight;

    // 頂点配列の作成（ローカル座標系の原点を中心にする）
    SpriteVertex vertices[] =
    {
        {
            VECTOR3(-m_fDestWidth / 2, m_fDestHeight / 2, 0),
            VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / dwImageWidth, (float)m_dwSrcY / dwImageHeight)
        }, // 左上
        {
            VECTOR3(-m_fDestWidth / 2, -m_fDestHeight / 2, 0),
            VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / dwImageWidth, (float)(m_dwSrcY + m_dwSrcHeight) / dwImageHeight)
        }, // 左下
        {
            VECTOR3(m_fDestWidth / 2, m_fDestHeight / 2, 0),
            VECTOR2((float)m_dwSrcX / dwImageWidth, (float)m_dwSrcY / dwImageHeight)
        }, // 右上
        {
            VECTOR3(m_fDestWidth / 2, -m_fDestHeight / 2, 0),
            VECTOR2((float)m_dwSrcX / dwImageWidth, (float)(m_dwSrcY + m_dwSrcHeight) / dwImageHeight)
        }, // 右下
    };

    // 頂点バッファの新規作成または動的更新
    if (m_pVertexBufferBillSprite == nullptr)
    {
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferBillSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4);
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferBillSprite, 0);
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------ // -- 2024.3.23
//
//  3Dビルボードオブジェクトのレンダリング
//
// -----------------------------------------------------------------------
bool CSprite::Draw3D(CSpriteImage* pImage, const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos,
                     const VECTOR2& vSrcSize, const float& fAlpha)
{
    return Draw3D(pImage, vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, vSize, vSrcPos,
                  vSrcSize, fAlpha);
}

bool CSprite::Draw3D(const VECTOR3& vPos, const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize,
                     const float& fAlpha)
{
    return Draw3D(vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, vSize, vSrcPos, vSrcSize,
                  fAlpha);
}

bool CSprite::Draw3D(const VECTOR3& vPos)
{
    return Draw3D(vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt);
}

bool CSprite::DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const DWORD& colorABGR, const float& fAlpha)
{
    return DrawLine3D(vStart, vEnd, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, colorABGR,
                      fAlpha);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool CSprite::Draw3DWithWorldMatrix(CSpriteImage* pImage, const MATRIX4X4& mWorld, const MATRIX4X4& mView,
                                    const MATRIX4X4& mProj, const VECTOR2& vSize, const VECTOR2& vSrcPos,
                                    const VECTOR2& vSrcSize, const float& fAlpha)
{
    m_pImage = pImage;

    if (m_pImage == nullptr) return false;

    // 頂点バッファのリセットと情報の更新
    SetSrc3D(vSize.x, vSize.y, (DWORD)vSrcPos.x, (DWORD)vSrcPos.y, (DWORD)vSrcSize.x, (DWORD)vSrcSize.y);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha;

    if (m_pVertexBufferBillSprite == nullptr) return false;

    // ビルボード用シェーダーをセット（ただし、この関数では独自の行列計算を使用）
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    // 頂点バッファをバインド
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferBillSprite, &stride, &offset);

    // ブレンドステートのセット
    UINT mask = 0xffffffff;
    if (m_nBlend == 1)
    {
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
    }
    else if (m_nBlend == 2)
    {
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, nullptr, mask);
    }

    // 3D上の半透明スプライトは、木などの手前オブジェクトには隠れるがZバッファには書き込まない。
    m_pD3D->m_pDeviceContext->OMSetDepthStencilState(m_pD3D->m_pDepthStencilStateNoWrite, 0);

    // 定数バッファの更新
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    ZeroMemory(&cb, sizeof(cb));

    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        // ビルボード計算ではなく、引数のワールド行列、ビュー、プロジェクションを直接合成
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        cb.vUVOffset.x = (float)m_ofX / m_pImage->m_dwImageWidth;
        cb.vUVOffset.y = (float)m_ofY / m_pImage->m_dwImageHeight;
        cb.vColor = m_vDiffuse;
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // テクスチャ使用フラグ

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    // 関連リソースのセット
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);

    // プリミティブ描画
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // 設定を元に戻す
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
    m_pD3D->m_pDeviceContext->OMSetDepthStencilState(m_pD3D->m_pDepthStencilStateDefault, 0);

    return true;
}

//------------------------------------------------------------------------
// 3D空間の地面などにスプライトを配置して描画する
// (キャラクターの足元の影や、魔法陣などの表示に使用)
//------------------------------------------------------------------------
bool CSprite::DrawWorld(CSpriteImage* pImage, const VECTOR3& vPos, float radius, float alpha)
{
    static constexpr float GROUND_OFFSET = 0.1f; // 地面との重なりによるチラつき（Zファイティング）防止
    static constexpr float CIRCLE_DIAMETER_SCALE = 2.0f; // 半径を直径に変換
    static constexpr float CIRCLE_DEPTH = 1.0f;
    static constexpr float GROUND_ROTATION = -XM_PI / 2.0f; // X軸で-90度回転させて地面と平行にする
    static constexpr float SPRITE_SIZE = 1.0f;

    const MATRIX4X4 mScale = XMMatrixScaling(radius * CIRCLE_DIAMETER_SCALE,
                                             radius * CIRCLE_DIAMETER_SCALE,
                                             CIRCLE_DEPTH);
    const MATRIX4X4 mRotation = XMMatrixRotationX(GROUND_ROTATION);
    const MATRIX4X4 mTranslation = XMMatrixTranslation(vPos.x, vPos.y + GROUND_OFFSET, vPos.z);

    // 行列を合成してワールド行列を作成
    const MATRIX4X4 mWorld = mScale * mRotation * mTranslation;

    return Draw3DWithWorldMatrix(
        pImage,
        mWorld,
        GameDevice()->m_mView,
        GameDevice()->m_mProj,
        VECTOR2(SPRITE_SIZE, SPRITE_SIZE),
        VECTOR2(0, 0),
        VECTOR2(static_cast<float>(pImage->m_dwImageWidth), static_cast<float>(pImage->m_dwImageHeight)),
        alpha
    );
}

//------------------------------------------------------------------------ // -- 2018.8.10
//
//  3D空間（ビルボード）にスプライトオブジェクトをレンダリング
//
//  頂点バッファを更新してから描画を行います。
//
//  引数：
//     CSpriteImage* pImage   スプライト画像
//     const VECTOR3& vPos       表示位置
//     const MATRIX4X4& mView    ビュー行列
//     const MATRIX4X4& mProj    投影行列
//     const VECTOR3& vEye       視点座標
//     const VECTOR2& vSize      表示サイズ
//     const VECTOR2& vSrcPos    切り取り位置
//     const VECTOR2& vSrcSize   切り取りサイズ
//     const float& fAlpha       透明度（初期値1.0f）
//
//  戻り値 bool：成功ならtrue
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
//  3D空間（ビルボード）にスプライトオブジェクトをレンダリング
//
//------------------------------------------------------------------------
bool CSprite::Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye,
                     const VECTOR2& vSize, const VECTOR2& vSrcPos, const VECTOR2& vSrcSize, const float& fAlpha)
{
    // 画像がセットされていない場合は描画を中止
    if (m_pImage == nullptr) return false;

    // 表示用のビルボード用頂点バッファを更新
    SetSrc3D(vSize.x, vSize.y, (DWORD)vSrcPos.x, (DWORD)vSrcPos.y, (DWORD)vSrcSize.x, (DWORD)vSrcSize.y);
    m_ofX = 0;
    m_ofY = 0;
    m_vDiffuse.w = fAlpha; // -- 2020.1.24

    return Draw3D(vPos, mView, mProj, vEye);
}

//------------------------------------------------------------------------ // -- 2018.8.10
//
//  3D空間（ビルボード）にスプライトオブジェクトをレンダリング
//
//  設定済みの頂点バッファを使用して描画を行います。
//
//  引数：
//     const VECTOR3&   vPos       表示位置
//     const MATRIX4X4& mView      ビュー行列
//     const MATRIX4X4& mProj      投影行列
//     const VECTOR3&   vEye       視点座標
//
//  戻り値 bool：成功ならtrue
//
//------------------------------------------------------------------------
bool CSprite::Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye)
{
    // リソースが足りない場合は描画不可
    if (m_pImage == nullptr || m_pVertexBufferBillSprite == nullptr) return false;

    // ビルボード計算：視点方向を向くワールド行列を取得
    MATRIX4X4 mWorld = GetLookatMatrix(vPos, vEye);

    // シェーダーをセット
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    // 頂点バッファをセット
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferBillSprite, &stride, &offset);

    // ブレンドステートの設定
    UINT mask = 0xffffffff;
    if (m_nBlend == 1)
    {
        // 半透明ブレンド
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
    }
    else if (m_nBlend == 2)
    {
        // 加算合成
        m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, nullptr, mask);
    }

    // 定数バッファの書き換え
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    ZeroMemory(&cb, sizeof(cb));

    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        // UVアニメーション等のオフセット設定
        cb.vUVOffset.x = (float)m_ofX / m_pImage->m_dwImageWidth;
        cb.vUVOffset.y = (float)m_ofY / m_pImage->m_dwImageHeight;
        cb.vColor = m_vDiffuse; // -- 2020.1.24
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // テクスチャ有効化フラグ

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    // 定数バッファ、レイアウト、トポロジー、サンプラー、テクスチャのバインド
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pImage->m_pTexture);

    // 描画
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // ブレンドステートを通常（透過）に戻す
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    return true;
} //------------------------------------------------------------------------ // -- 2018.8.10
//
//  3D空間へのラインレンダリング
//
//  設定済みの頂点バッファを使用して描画を行います。
//
//  （注意：色と透明度は m_vDiffuse を使用せず、引数で指定されたものを使用）
//
//  引数：
//     const VECTOR3& vStart     ラインの開始位置
//     const VECTOR3& vEnd       ラインの終了位置
//     const MATRIX4X4& mView    ビュー行列
//     const MATRIX4X4& mProj    投影行列
//     const VECTOR3& vEye       視点座標
//     const DWORD& colorABGR    描画色（0xAABBGR形式）
//     const float& fAlpha       透明度（初期値1.0f）
//
//  戻り値 bool：成功ならtrue
//
//------------------------------------------------------------------------
bool CSprite::DrawLine3D(const VECTOR3& vStart, const VECTOR3& vEnd, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                         const VECTOR3& vEye, const DWORD& colorABGR, const float& fAlpha)
{
    // 使用するシェーダーをセット
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    // ライン用の頂点配列
    SpriteVertex vertices[] =
    {
        {VECTOR3(vStart.x, vStart.y, vStart.z), VECTOR2(0, 0)}, // 頂点1
        {VECTOR3(vEnd.x, vEnd.y, vEnd.z), VECTOR2(0, 0)} // 頂点2
    };

    // 頂点バッファが未作成なら新規作成、作成済みならMap/Unmapで更新
    if (m_pVertexBufferLine == nullptr)
    {
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 2;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBufferLine)))
        {
            MessageBox(0, _T("Sprite3D.cpp 頂点バッファ作成失敗(LINE3D)"), nullptr, MB_OK);
            return false;
        }
    }
    else
    {
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBufferLine, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 2);
            m_pD3D->m_pDeviceContext->Unmap(m_pVertexBufferLine, 0);
        }
    }

    // 色データの正規化 (0~255 -> 0.0~1.0)
    VECTOR4 color;
    color.x = ((colorABGR & 0x000000ff) >> 0) / 255.0f; // R
    color.y = ((colorABGR & 0x0000ff00) >> 8) / 255.0f; // G
    color.z = ((colorABGR & 0x00ff0000) >> 16) / 255.0f; // B
    color.w = fAlpha; // A

    // バッファをセット
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBufferLine, &stride, &offset);
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);

    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    // テクスチャは無効
    ID3D11ShaderResourceView* Nothing[1] = {0};
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

    // 行列計算（ライン自体の位置は頂点座標で持っているため、ワールド行列は単位行列）
    MATRIX4X4 mWorld = XMMatrixIdentity();

    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        cb.vUVOffset = VECTOR2(0, 0);
        cb.vColor = color;
        cb.vMatInfo = VECTOR4(0, 0, 0, 0); // テクスチャなしモード

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);

    // 描画実行（2頂点）
    m_pD3D->m_pDeviceContext->Draw(2, 0);

    return true;
}

// ========================================================================================
//
// フォントテクスチャ描画クラス
//
// ========================================================================================
//------------------------------------------------------------------------
//
//  フォントテクスチャのコンストラクタ    
//
//  引数：CShader* pShader
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
    // 外部のTTFフォントファイルを読み込んで利用する場合
    DESIGNVECTOR design;
    if (AddFontResourceEx(
        TEXT("Fonts/外部フォント名.ttf"),
        FR_PRIVATE,
        &design) == nullptr)
    {
        MessageBox(0, _T("Sprite.cpp フォント読み込みに失敗しました"), _T(""), MB_OK);
    }
    */

    // 3Dテキスト表示用設定
    m_fDestWidth = 0.0f;
    m_fDestHeight = 0.0f;
}

//------------------------------------------------------------------------
//
//  フォントテクスチャのデストラクタ  
//
//------------------------------------------------------------------------
CFontTexture::~CFontTexture()
{
    /*
    // 読み込んだ外部フォントを解除
    DESIGNVECTOR design;
    RemoveFontResourceEx(
        TEXT("Fonts/外部フォント名.ttf"),
        FR_PRIVATE,
        &design);
    */

    for (DWORD i = 0; i < TEXT_DATA_MAX; i++)
    {
        SAFE_DELETE_ARRAY(m_TextData[i].m_szText);
        SAFE_RELEASE(m_TextData[i].m_pResourceView);
        SAFE_RELEASE(m_TextData[i].m_pVertexBufferFont);
    }
}

//------------------------------------------------------------------------
// テキストデータのインデックスリフレッシュ
// 
//   表示するテキスト情報をクリアし、配列のインデックスを先頭に戻します。
//   毎フレームの描画ループの最初で呼び出す必要があります。
//
//  引数：なし
//  戻り値：なし
//
//------------------------------------------------------------------------
void CFontTexture::Refresh()
{
    m_Idx = 0;
}

//------------------------------------------------------------------------
//
//  テキスト表示用の頂点バッファを作成（更新）
//  1行分の四角形ポリゴンを生成します。
//
//  const DWORD& dwWidth    表示幅（1行の長さ）
//  const DWORD& dwHeight   表示高さ（フォントの高さ）
//
//  戻り値：なし
//
//------------------------------------------------------------------------ 
void CFontTexture::CreateVB(const DWORD& dwWidth, const DWORD& dwHeight)
{
    // 頂点バッファデータを作成。z値は1.0で固定。
    // トライアングルストリップ形式（左下、左上、右下、右上）
    SpriteVertex vertices[] =
    {
        {VECTOR3(0, (float)dwHeight, 0), VECTOR2(0, 1)}, // 頂点1：左下
        {VECTOR3(0, 0, 0), VECTOR2(0, 0)}, // 頂点2：左上
        {VECTOR3((float)dwWidth, (float)dwHeight, 0), VECTOR2(1, 1)}, // 頂点3：右下
        {VECTOR3((float)dwWidth, 0, 0), VECTOR2(1, 0)} // 頂点4：右上
    };

    // 頂点バッファが未作成の場合は新規作成、作成済みの場合はMapで更新
    if (m_TextData[m_Idx].m_pVertexBufferFont == nullptr)
    {
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_TextData[m_Idx].m_pVertexBufferFont)))
        {
            MessageBox(0, _T("Sprite.cpp フォント用頂点バッファ作成失敗"), nullptr, MB_OK);
            return;
        }
    }
    else
    {
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_TextData[m_Idx].m_pVertexBufferFont, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4);
            m_pD3D->m_pDeviceContext->Unmap(m_TextData[m_Idx].m_pVertexBufferFont, 0);
        }
    }
}

//------------------------------------------------------------------------
//
//  テキストの描画（スクリーン座標指定）
//
//  float posX         表示位置X座標
//  float posY         表示位置Y座標
//  const TCHAR* szText       表示する文字列
//  int fontsize       フォントサイズ
//  DWORD colorABGR    表示色（0xAABBGR形式）
//  float fAlpha       透明度（初期値1.0f）
//  const TCHAR* szFontName   フォント名（省略可）
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
//  テキストの描画（ワールド行列指定）
//
//  MATRIX4X4 mWorld  表示位置用ワールドマトリックス
//  const TCHAR* szText       表示する文字列
//  int fontsize       フォントサイズ
//  DWORD colorABGR    表示色
//  float fAlpha       透明度
//  const TCHAR* szFontName   フォント名
//
//------------------------------------------------------------------------ 
void CFontTexture::Draw(MATRIX4X4 mWorld, const TCHAR* szText, int fontsize, DWORD colorABGR, float fAlpha,
                        const TCHAR* szFontName)
{
    // 文字列が空の場合は描画しない
    if (szText[0] == _T('\0')) return;

    // 前回のデータと比較し、変更がある場合のみテクスチャを再生成する
    // (文字列、サイズ、色、透明度が変わっていなければ既存のテクスチャを再利用)
    if (m_TextData[m_Idx].m_szText == nullptr || _tcscmp(m_TextData[m_Idx].m_szText, szText) != 0 ||
        m_TextData[m_Idx].m_iFontsize != fontsize || m_TextData[m_Idx].m_dwColor != colorABGR || m_TextData[m_Idx].
        m_fAlpha != fAlpha)
    {
        SAFE_DELETE_ARRAY(m_TextData[m_Idx].m_szText);
        m_TextData[m_Idx].m_szText = new TCHAR[_tcslen(szText) + 1];
        m_TextData[m_Idx].m_dwKbn = 0; // 2Dフォントフラグ
        _tcscpy_s(m_TextData[m_Idx].m_szText, _tcslen(szText) + 1, szText);

        m_TextData[m_Idx].m_iFontsize = fontsize;
        m_TextData[m_Idx].m_dwColor = colorABGR;
        m_TextData[m_Idx].m_fAlpha = fAlpha;

        // フォントテクスチャの作成実行
        CreateTex(0, 0, 0, szText, fontsize, colorABGR, szFontName);
    }

    // 描画ステートの設定
    SetShader();

    // 作成済みの頂点バッファをセット
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_TextData[m_Idx].m_pVertexBufferFont, &stride, &offset);

    // 定数バッファの更新
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        cb.mW = XMMatrixTranspose(mWorld);
        cb.ViewPortWidth = (float)m_pD3D->m_dwWindowWidth;
        cb.ViewPortHeight = (float)m_pD3D->m_dwWindowHeight;
        cb.vUVOffset = VECTOR2(0, 0);
        cb.vColor = VECTOR4(1, 1, 1, m_TextData[m_Idx].m_fAlpha);
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // テクスチャ有効

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    // テクスチャ（SRV）をセットして描画
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_TextData[m_Idx].m_pResourceView);
    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // ステートを元に戻す
    ResetShader();

    // 次の描画のためにインデックスを進める
    m_Idx++;
    if (m_Idx >= TEXT_DATA_MAX)
    {
        MessageBox(0, _T("描画可能なテキスト数の上限(TEXT_DATA_MAX)を超えました。\nRefresh()の呼び出し位置、または最大数設定を確認してください。"), _T("Error"),
                   MB_OK);
        m_Idx--;
    }
}

//------------------------------------------------------------------------
//
//  描画開始時のシェーダー設定
//
//------------------------------------------------------------------------
void CFontTexture::SetShader()
{
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

    // 半透明ブレンドを有効化
    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    // テキストは最前面に描画するためZバッファ無効化
    m_pD3D->SetZBuffer(false);
}

//------------------------------------------------------------------------
//
//  描画終了時のシェーダーリセット
//
//------------------------------------------------------------------------
void CFontTexture::ResetShader()
{
    m_pD3D->SetZBuffer(true);

    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);
}

//------------------------------------------------------------------------
//
//  フォントテクスチャの作成
//
//  GDIを用いてフォントをグラフィックスデバイスに描き、テクスチャ化する
//  作成したテクスチャは m_TextData[m_Idx] 配列に格納される
//
//  const DWORD&  dwKbn        区分 0:2Dフォント  1:3Dフォント  // -- 2018.8.10
//  const float&  fDestWidth  3D表示時の幅                      // -- 2018.8.10
//  const float&  fDestHeight 3D表示時の高さ                    // -- 2018.8.10
//  const TCHAR* text         表示するテキスト
//  const int&    fontsize     フォントサイズ
//  const DWORD&  colorABGR    表示色（0xAABBGR形式）
//  const TCHAR* fontname     フォント名（nullptrの場合はデフォルト）
//
//  戻り値：なし
//------------------------------------------------------------------------ 
void CFontTexture::CreateTex(const DWORD& dwKbn, const float& fDestWidth, const float& fDestHeight, const TCHAR* text,
                             const int& fontsize, const DWORD& colorABGR, const TCHAR* fontname)
{
    // フォントの設定
    LOGFONT lf = {
        fontsize,
        0, 0, 0, 0, 0, 0, 0,
        SHIFTJIS_CHARSET,
        OUT_TT_ONLY_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        FIXED_PITCH | FF_MODERN,
        TEXT("ＭＳ ゴシック") // デフォルトフォント名
    };

    if (fontname != nullptr)
        _tcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(TCHAR), fontname);

    HFONT hFont = CreateFontIndirect(&lf);
    if (!(hFont))
    {
        MessageBox(0, _T("Sprite.cpp テクスチャ用フォント作成失敗"), _T(""), MB_OK);
    }

    // 文字列のサイズ計測とグリフ情報の取得
    DWORD dwTextlen = (DWORD)_tcslen(text);
    DWORD dwTextHeight = 0;
    DWORD dwTextWidth = 0;
    DWORD dwAllWidth = 0;
    DWORD dwAllWidth2 = 0;
    FontData* pFontData = new FontData[dwTextlen];

    // デバイスコンテキストを取得し、フォントを適用
    HDC hdc = GetDC(nullptr);
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    for (DWORD i = 0; i < dwTextlen; i++)
    {
        UINT code = 0;
#if _UNICODE
        code = (UINT)*(text + i);
#else
        BYTE* c = (BYTE*)(text + i);
        if (IsDBCSLeadByte(*c))
        {
            code = (BYTE)c[0] << 8 | (BYTE)c[1];
            i++;
        }
        else
        {
            code = c[0];
        }
#endif

        // グリフ情報の取得
        GetTextMetrics(hdc, &(pFontData + i)->TM);
        CONST MAT2 Mat = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
        DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &(pFontData + i)->GM, 0, nullptr, &Mat);

        if (size == 0)
        {
            (pFontData + i)->ptr = nullptr;
        }
        else
        {
            (pFontData + i)->ptr = new BYTE[size];
            GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &(pFontData + i)->GM, size, (pFontData + i)->ptr, &Mat);
        }

        if ((int)dwTextWidth < (pFontData + i)->GM.gmCellIncX) dwTextWidth = (pFontData + i)->GM.gmCellIncX;
        if (dwTextHeight < (DWORD)(pFontData + i)->TM.tmHeight) dwTextHeight = (pFontData + i)->TM.tmHeight;
        if (dwTextHeight < (DWORD)(pFontData + i)->GM.gmBlackBoxY) dwTextHeight = (pFontData + i)->GM.gmBlackBoxY;

        dwAllWidth += (pFontData + i)->GM.gmCellIncX;
        dwAllWidth2 += (pFontData + i)->GM.gmBlackBoxX + (4 - ((pFontData + i)->GM.gmBlackBoxX % 4)) % 4;
    }

    if (dwAllWidth < dwAllWidth2) dwAllWidth = dwAllWidth2;

    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    ReleaseDC(nullptr, hdc);

    // 区分に応じて頂点バッファを作成
    if (dwKbn == 0) CreateVB(dwAllWidth, dwTextHeight);
    else CreateVB3D(fDestWidth, fDestHeight);

    // D3D11 動的テクスチャの作成
    ID3D11Texture2D* pTexture2D;
    D3D11_TEXTURE2D_DESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.Width = dwAllWidth;
    desc.Height = dwTextHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(m_pD3D->m_pDevice->CreateTexture2D(&desc, 0, &pTexture2D)))
    {
        MessageBox(0, _T("Sprite.cpp フォントテクスチャ生成失敗"), _T(""), MB_OK);
    }

    // テクスチャをロックしてフォントビットマップを書き込む
    D3D11_MAPPED_SUBRESOURCE hMappedResource;
    if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedResource)))
    {
        BYTE* pBits = (BYTE*)hMappedResource.pData;
        ZeroMemory(pBits, hMappedResource.RowPitch * dwTextHeight);

        DWORD dwAllWidthWk = 0;
        for (DWORD i = 0; i < dwTextlen; i++)
        {
            int iOfs_x = (pFontData + i)->GM.gmptGlyphOrigin.x;
            int iOfs_y = (pFontData + i)->TM.tmAscent - (pFontData + i)->GM.gmptGlyphOrigin.y;
            int iBmp_w = (pFontData + i)->GM.gmBlackBoxX + (4 - ((pFontData + i)->GM.gmBlackBoxX % 4)) % 4;
            int iBmp_h = (pFontData + i)->GM.gmBlackBoxY;
            if (iOfs_x < 0) iOfs_x = 0;
            if (iOfs_y < 0) iOfs_y = 0;
            int Level = 17; // GGO_GRAY4_BITMAPの階調レベル

            for (int y = iOfs_y; y < iOfs_y + iBmp_h; y++)
            {
                if ((DWORD)y >= dwTextHeight) break;
                for (int x = iOfs_x; x < iOfs_x + iBmp_w; x++)
                {
                    DWORD Alpha, Color;
                    if ((pFontData + i)->ptr == nullptr)
                    {
                        Alpha = 0;
                    }
                    else
                    {
                        // 階調値を255スケールに変換
                        Alpha = (255 * (pFontData + i)->ptr[(x - iOfs_x) + iBmp_w * (y - iOfs_y)]) / (Level - 1);
                    }
                    Color = (colorABGR & 0x00ffffff) | (Alpha << 24);

                    memcpy((BYTE*)pBits + hMappedResource.RowPitch * y + 4 * (x + dwAllWidthWk), &Color, sizeof(DWORD));
                }
            }
            dwAllWidthWk += (pFontData + i)->GM.gmCellIncX;
        }
        m_pD3D->m_pDeviceContext->Unmap(pTexture2D, 0);
    }

    // シェーダーリソースビューの作成
    SAFE_RELEASE(m_TextData[m_Idx].m_pResourceView);
    m_pD3D->m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_TextData[m_Idx].m_pResourceView);

    SAFE_RELEASE(pTexture2D);
    for (DWORD i = 0; i < dwTextlen; i++) SAFE_DELETE_ARRAY((pFontData + i)->ptr);
    SAFE_DELETE_ARRAY(pFontData);
}


//------------------------------------------------------------------------ // -- 2018.8.10
//
//  3Dビルボードフォント用の頂点バッファ設定
//
//  const float& fDestWidth         表示幅
//  const float& fDestHeight        表示高さ
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 失敗
//------------------------------------------------------------------------
HRESULT CFontTexture::CreateVB3D(const float& fDestWidth, const float& fDestHeight)
{
    // ビルボード用ポリゴンの設定（中心を基準に作成）
    m_fDestWidth = fDestWidth;
    m_fDestHeight = fDestHeight;

    // 頂点配列（トライアングルストリップ形式）
    SpriteVertex vertices[] =
    {
        {VECTOR3(-m_fDestWidth / 2, m_fDestHeight / 2, 0), VECTOR2(0, 0)}, // 左上
        {VECTOR3(-m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2(0, 1)}, // 左下
        {VECTOR3(m_fDestWidth / 2, m_fDestHeight / 2, 0), VECTOR2(1, 0)}, // 右上
        {VECTOR3(m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2(1, 1)}, // 右下
    };

    if (m_TextData[m_Idx].m_pVertexBufferFont == nullptr)
    {
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = vertices;
        if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_TextData[m_Idx].m_pVertexBufferFont)))
        {
            MessageBox(0, _T("Sprite.cpp フォント用3D頂点バッファ作成失敗"), nullptr, MB_OK);
            return E_FAIL;
        }
    }
    else
    {
        D3D11_MAPPED_SUBRESOURCE msr;
        if (SUCCEEDED(
            m_pD3D->m_pDeviceContext->Map(m_TextData[m_Idx].m_pVertexBufferFont, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
        {
            memcpy(msr.pData, vertices, sizeof(SpriteVertex) * 4);
            m_pD3D->m_pDeviceContext->Unmap(m_TextData[m_Idx].m_pVertexBufferFont, 0);
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------ // -- 2024.3.23
//
//  3Dビルボードフォントオブジェクトのレンダリング
//
//------------------------------------------------------------------------ 
bool CFontTexture::Draw3D(const VECTOR3& vPos, const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR,
                          const float& fAlpha, const TCHAR* szFontName)
{
    // 現在のビュー、プロジェクション、視点座標を使用して描画
    return Draw3D(vPos, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vEyePt, szText, vSize, colorABGR,
                  fAlpha, szFontName);
}

//------------------------------------------------------------------------ // -- 2018.8.10
//
//  3Dビルボードフォントオブジェクトのレンダリング（詳細設定）
//
//  引数：
//     const VECTOR3& vPos     表示座標
//     const MATRIX4X4& mView  ビューマトリックス
//     const MATRIX4X4& mProj  プロジェクションマトリックス
//     const VECTOR3& vEye     視点座標
//     const TCHAR* szText     表示する文字列
//     const VECTOR2& vSize    表示サイズ（x:幅 y:高さ）
//     const DWORD& colorABGR   表示色（0xAABBGR）
//     const float& fAlpha      透明度
//     const TCHAR* szFontName フォント名
//
//  戻り値 bool：表示成功ならtrue
//
//------------------------------------------------------------------------
bool CFontTexture::Draw3D(const VECTOR3& vPos, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vEye,
                          const TCHAR* szText, const VECTOR2& vSize, const DWORD& colorABGR, const float& fAlpha,
                          const TCHAR* szFontName)
{
    // 文字列が空の場合は描画をスキップ
    if (szText[0] == _T('\0')) return false;

    // 前回の描画データ（文字列、サイズ、色、透明度）と比較し、
    // 変更がある場合、または初めて描画する場合にテクスチャを再生成する。
    // ※ 3Dフォントの場合は m_dwKbn = 1 を使用
    DWORD fontsize = (DWORD)vSize.y * 100;

    // 複数の状態や境界条件をまとめて判定する。
    if (m_TextData[m_Idx].m_dwKbn != 1 || m_TextData[m_Idx].m_szText == nullptr || _tcscmp(
            m_TextData[m_Idx].m_szText, szText) != 0 ||
        m_TextData[m_Idx].m_iFontsize != fontsize || m_TextData[m_Idx].m_dwColor != colorABGR || m_TextData[m_Idx].
        m_fAlpha != fAlpha)
    {
        // 既存の文字列バッファを削除して新しく確保
        SAFE_DELETE_ARRAY(m_TextData[m_Idx].m_szText);
        m_TextData[m_Idx].m_szText = new TCHAR[_tcslen(szText) + 1];
        m_TextData[m_Idx].m_dwKbn = 1; // 3Dフォント区分
        _tcscpy_s(m_TextData[m_Idx].m_szText, _tcslen(szText) + 1, szText);

        m_TextData[m_Idx].m_iFontsize = fontsize;
        m_TextData[m_Idx].m_dwColor = colorABGR;
        m_TextData[m_Idx].m_fAlpha = fAlpha;

        // 指定条件でフォントテクスチャを作成
        CreateTex(1, vSize.x, vSize.y, szText, fontsize, colorABGR, szFontName);
    }

    // ビルボード計算：視点方向を向くワールド行列を生成
    MATRIX4X4 mWorld = GetLookatMatrix(vPos, vEye);

    // シェーダーをセット（ビルボード用VSを使用）
    m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSprite3D_VS_BILL, nullptr, 0);
    m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSprite3D_PS, nullptr, 0);

    // 頂点バッファをセット
    UINT stride = sizeof(SpriteVertex);
    UINT offset = 0;
    m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_TextData[m_Idx].m_pVertexBufferFont, &stride, &offset);

    // 半透明ブレンドを有効化
    UINT mask = 0xffffffff;
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    // 定数バッファのMap
    D3D11_MAPPED_SUBRESOURCE pData;
    CONSTANT_BUFFER_SPRITE cb;
    ZeroMemory(&cb, sizeof(cb));

    if (SUCCEEDED(
        m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferSprite3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
    {
        // 転置した合成行列（WVP）をセット
        cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

        cb.vUVOffset.x = 0;
        cb.vUVOffset.y = 0;
        cb.vColor = VECTOR4(1, 1, 1, 1);
        cb.vColor.w = fAlpha;
        cb.vMatInfo = VECTOR4(1, 0, 0, 0); // テクスチャ使用モード

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferSprite3D, 0);
    }

    // シェーダーリソースのセットと描画
    m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferSprite3D);
    m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSprite3D_VertexLayout);
    m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
    m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_TextData[m_Idx].m_pResourceView);

    m_pD3D->m_pDeviceContext->Draw(4, 0);

    // ブレンドステートをリセット
    m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, nullptr, mask);

    // テキスト管理配列のインデックスを進める
    m_Idx++;
    if (m_Idx >= TEXT_DATA_MAX)
    {
        MessageBox(
            0, _T(
                "テキストデータの最大数(TEXT_DATA_MAX)を超えました。\nRefresh()の呼び出し位置、または最大数設定を確認してください。"),
            _T("Error"), MB_OK);
        m_Idx--;
    }

    return true;
}
