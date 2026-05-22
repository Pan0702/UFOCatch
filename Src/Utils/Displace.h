// ========================================================================================
//
//   ディスプレイスメントマッピングを利用するライブラリ
//                                                                 ver 3.3
//
//   波の効果を設定
//
//      (波の揺れ、地表の凹凸の表現に対応)
//
//                                                            2024.3.23
// ========================================================================================

#pragma once

#include "../Core/Graphics/Direct3D.h"
#include "../Core/Game/GameMain.h"

//
// 頂点の構造体
//
struct DISPLACE_VERTEX
{
    VECTOR3 vPos; // 座標
    VECTOR2 vTex; // テクスチャ座標
    VECTOR3 vNorm; // 法線
};

// 
class CDirect3D;
/// <summary>Direct3D用シェーダと定数バッファを管理するクラス</summary>
class CShader;

//
// ディスプレイスメント波のクラス
//
class CWave
{
public:
    // Direct3D11関連
    CDirect3D* m_pD3D;
    CShader* m_pShader;

    // リソースバッファ
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pIndexBuffer;
    ID3D11ShaderResourceView* m_pTexture; // ディフューズテクスチャ
    ID3D11ShaderResourceView* m_pNormalTexture; // ノーマルテクスチャ（ハイトマップ兼用）
    ID3D11ShaderResourceView* m_pSpecularTexture; // スペキュラーテクスチャ // -- 2021.2.4

    DWORD m_dwNumIndex; // インデックス数
    VECTOR4 m_vWaveMove; // 波の移動量
    VECTOR2 m_vWaveSpeed; // 波の移動速度
    FLOAT m_fWaveHeight; // 波の高さ
    VECTOR4 m_vDiffuse; // ディフューズ色
    VECTOR4 m_vSpecular; // スペキュラー色

    FLOAT m_fMinDistance; // 最小距離（分割最大地点）
    FLOAT m_fMaxDistance; // 最大距離（分割開始地点）
    int m_iMaxDevide; // 最大分割数

public:
    /// CWave を初期化する
    CWave(); // -- 2024.3.23
    /// CWave を初期化する
    /// @param pShader pShader に渡す値
    CWave(CShader* pShader);
    /// CWave を初期化する
    /// @param iWidth 幅
    /// @param iHeight 高さ
    /// @param FName 名前
    /// @param TexName 名前
    /// @param SpecName 名前
    CWave(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName = nullptr,
          const TCHAR* SpecName = nullptr); // -- 2024.3.23
    /// CWave を初期化する
    /// @param pShader pShader に渡す値
    /// @param iWidth 幅
    /// @param iHeight 高さ
    /// @param FName 名前
    /// @param TexName 名前
    /// @param SpecName 名前
    CWave(CShader* pShader, const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName = nullptr,
          const TCHAR* SpecName = nullptr); // -- 2021.2.4
    /// CWave の終了処理を行う
    ~CWave();

    // メンバ関数
    /// 初期化する
    void Init();
    /// All を削除する
    void DeleteAll();
    /// Src を設定する
    /// @param iWidth 幅
    /// @param iHeight 高さ
    /// @param FName 名前
    /// @param TexName 名前
    /// @param SpecName 名前
    /// @return 処理結果
    HRESULT SetSrc(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName,
                   const TCHAR* SpecName); // -- 2021.2.4
    /// 描画する
    /// @param mWorld mWorld に渡す値
    void Render(const MATRIX4X4& mWorld); // -- 2024.3.23
    /// 描画する
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight,
                const VECTOR3& vEye);
};
