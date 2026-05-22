//=============================================================================
//		メッシュの読み込みと描画のプログラム
//　                                                  ver 4.0        2024.10.7
//
//		メッシュ処理
//
//      (メッシュコントロールクラスでテクスチャの総合管理を行う)
//
//
//																	FbxMesh.h
//=============================================================================
#pragma once


//=============================================================================
//		メッシュの読み込みと描画のプログラム
//　                                                  ver 4.1        2025.6.24
//
//		メッシュ処理
//
//      (メッシュコントロールクラスでテクスチャの総合管理を行う)
//
//
//																	FbxMesh.h
//=============================================================================
#pragma once

#include <list>
#include "../Core/Graphics/Direct3D.h"
#include "../Framework/Shader.h"

////警告非表示
//#pragma warning(disable : 4005)
//#pragma warning(disable : 4244)
//#pragma warning(disable : 4018)
//
//
#define MAX_BONES 255			// 最大ボーン数
#define ANIMATION_MAX 50		// 最大アニメーション数

// テクスチャリスト                                  // -- 2021.2.4
struct TextureList
{
    TCHAR m_FName[MAX_PATH]; // テクスチャ名
    ID3D11ShaderResourceView* m_pTexture; // テクスチャ
    int m_nCnt; // このテクスチャを使用しているメッシュの数

    /// TextureList を初期化する
    TextureList()
    {
        m_FName[0] = _T('\0');
        m_pTexture = nullptr;
        m_nCnt = 0;
    }
};

// スタティックメッシュ頂点の構造体
struct StaticVertex
{
    VECTOR3 Pos; // 位置
    VECTOR3 Normal; // 法線　シェーディング（陰影計算）には法線は必須
    VECTOR2 vTex; // テクスチャー座標
};

/// <summary>汎用ユーティリティで使う Static Vertex Normal の情報と処理をまとめる型</summary>
struct StaticVertexNormal
{
    VECTOR3 Pos; // 位置
    VECTOR3 Normal; // 法線　シェーディング（陰影計算）には法線は必須
    VECTOR2 vTex; // テクスチャー座標
    VECTOR3 Tangent; // 接ベクトル  ノーマルマッピングには接ベクトルが必要
    VECTOR3 Binormal; // 従法線      ノーマルマッピングには従法線が必要
};

// スキンメッシュ頂点の構造体
struct SkinVertex
{
    VECTOR3 Pos; //位置
    VECTOR3 Normal; //法線　シェーディング（陰影計算）には法線は必須
    VECTOR2 vTex; //テクスチャー座標
    UINT ClusterNum[4]; // 影響を与えるクラスター(ボーン)インデックス
    VECTOR4 Weits; // ウェイト
};

/// <summary>汎用ユーティリティで使う Skin Vertex Normal の情報と処理をまとめる型</summary>
struct SkinVertexNormal
{
    VECTOR3 Pos; //位置
    VECTOR3 Normal; //法線　シェーディング（陰影計算）には法線は必須
    VECTOR2 vTex; //テクスチャー座標
    UINT ClusterNum[4]; // 影響を与えるクラスター(ボーン)インデックス
    VECTOR4 Weits; // ウェイト
    VECTOR3 Tangent; //接ベクトル
    VECTOR3 Binormal; //従法線
};

// ボーン構造体
struct BONE
{
    MATRIX4X4 bindPose; // 初期ポーズ(逆行列状態)
    int framePoseNum; // フレームポーズ数
    MATRIX4X4* framePose; // フレームポーズ

    BONE()
    {
        framePose = nullptr;
        framePoseNum = 0;
        framePose = nullptr;
    }

    /// BONE の終了処理を行う
    ~BONE()
    {
        SAFE_DELETE_ARRAY(framePose);
    }
};

// アニメーション構造体
struct ANIMATION
{
    bool used;
    int startFrame;
    int endFrame;
    int RootBoneMesh; // -- 2020.12.15 -- 3
    int RootBone; // -- 2020.12.15 -- 3
    bool loop; // ループフラグをここに持たせる
    ANIMATION()
    {
        used = false;
        startFrame = 0;
        endFrame = 0;
        RootBoneMesh = 0;
        RootBone = 0;
        loop = false;
    }
};

// シェーダにそのまま渡せるようにボーン基準に整列した合成済み行列の配列
struct BONESHADER
{
    MATRIX4X4 shaderFramePose[MAX_BONES];
};

// ルートボーンアニメーションタイプ
enum ROOTANIMTYPE { eRootAnimNone = 0, eRootAnimXZ = 1, eRootAnim = 2 }; // -- 2020.12.15 -- 3

// レンダリング順の構造体			  // -- 2025.3.15
struct RENDERORDER
{
    DWORD Idx; // レンダリング順の添字
    float Distance; // レンダリング順を決めるための距離
    /// operator< の処理を行う
    /// @param another another に渡す値
    /// @return 成功または条件を満たす場合 true
    bool operator<(const RENDERORDER& another) const
    {
        //メンバ変数であるDistanceで比較した結果を
        //この構造体の比較とする
        return Distance > another.Distance;
    }
};

// メッシュ配列
struct CFbxMeshArray
{
    StaticVertexNormal* m_vStaticVerticesNormal; // 頂点データ バンプマップ用
    SkinVertexNormal* m_vSkinVerticesNormal; // 頂点データ バンプマップ用
    DWORD* m_nIndices; // インデックスデータ
    ID3D11Buffer* m_pVertexBuffer; // バーテックスバッファ
    ID3D11Buffer* m_pIndexBuffer; // インデックスバッファ
    DWORD m_dwVerticesNum; // バーテックスバッファ要素数
    DWORD m_dwIndicesNum; // インデックスバッファ要素数
    VECTOR4 m_pMaterialDiffuse; // マテリアルディフューズカラー     // -- 2020.12.15
    VECTOR4 m_pMaterialSpecular; // マテリアルスペキュラカラー       // -- 2020.12.15
    ID3D11ShaderResourceView* m_pTexture; // テクスチャーアドレス
    ID3D11ShaderResourceView* m_pTextureNormal; // テクスチャーNormalアドレス
    ID3D11ShaderResourceView* m_pTextureHeight; // テクスチャーHeightアドレス
    ID3D11ShaderResourceView* m_pTextureSpecular; // テクスチャーSpecularアドレス
    DWORD m_NumBones; // ボーン数
    BONE m_BoneArray[ANIMATION_MAX][MAX_BONES]; // アニメーションデータが[フレーム数][ボーン数] 
    BONESHADER* m_pBoneShader[ANIMATION_MAX]; // シェーダーに渡すボーン行列[フレーム数]

    VECTOR3 m_vCenterPos; // メッシュ中心点座標

    /// CFbxMeshArray を初期化する
    CFbxMeshArray()
    {
        m_vStaticVerticesNormal = nullptr;
        m_vSkinVerticesNormal = nullptr;
        m_nIndices = nullptr;
        m_pVertexBuffer = nullptr;
        m_pIndexBuffer = nullptr;
        m_dwVerticesNum = 0;
        m_dwIndicesNum = 0;
        m_pTexture = nullptr;
        m_pTextureNormal = nullptr;
        m_pTextureHeight = nullptr;
        m_pTextureSpecular = nullptr;
        m_NumBones = 0; // ボーン数
        ZeroMemory(m_BoneArray, sizeof(m_BoneArray));
        ZeroMemory(m_pBoneShader, sizeof(m_pBoneShader));
        m_vCenterPos = VECTOR3(0, 0, 0);
    }
};

//
// Fbxメッシュクラス
//
class CShader;
/// <summary>汎用ユーティリティで使う Animator の情報と処理をまとめる型</summary>
class Animator;
/// <summary>汎用ユーティリティで使う Fbx Mesh Ctrl の情報と処理をまとめる型</summary>
class CFbxMeshCtrl;

/// <summary>汎用ユーティリティで使う Fbx Mesh の情報と処理をまとめる型</summary>
class CFbxMesh
{
public:
    CDirect3D* m_pD3D; // Direct3D11
    CShader* m_pShader;
    CFbxMeshCtrl* m_pFbxMeshCtrl; // テクスチャコントロールクラス		// -- 2021.2.4

    int m_nMeshType; // 0:未定  1:スタティックメッシュ  2:スキンメッシュ
    VECTOR4* m_pMaterialDiffuseArray; // マテリアルディフューズカラー     // -- 2020.12.15
    VECTOR4* m_pMaterialSpecularArray; // マテリアルスペキュラカラー       // -- 2020.12.15
    ID3D11ShaderResourceView** m_pTextureArray; //テクスチャー配列
    ID3D11ShaderResourceView** m_pTextureNormalArray; //テクスチャーNormal配列
    ID3D11ShaderResourceView** m_pTextureHeightArray; //テクスチャーHeight配列
    ID3D11ShaderResourceView** m_pTextureSpecularArray; // テクスチャーSpecular配列
    DWORD m_dwTextureNum; //テクスチャー配列要素数

    CFbxMeshArray* m_pMeshArray; // メッシュオブジェクト配列
    DWORD m_dwMeshNum; // メッシュオブジェクトの要素数
    //	int                   m_allAnimationCount;  // 全てのアニメーション数

    ANIMATION m_Animation[ANIMATION_MAX]; // アニメーション
    BONE m_RootBoneArray[ANIMATION_MAX]; // ルートボーンアニメーションデータが[フレーム数]分   // -- 2020.12.15 -- 3
    ROOTANIMTYPE m_RootAnimType[ANIMATION_MAX];
    // ルートボーンアニメタイプ   eRootAnimNone:ルートアニメなし  eRootAnimXZ:ルートアニメXZ  eRootAnim:ルートアニメ  // -- 2020.12.15 -- 3

    float m_fHeightMax; // ディスプレイスメントマッピングの高さ
    int m_iMaxDevide; // ディスプレイスメントマッピングの分割数
    VECTOR4 m_vDiffuse; // ディフューズカラー

    DWORD m_dwLoadTime; // ロード時間

    std::vector<RENDERORDER> m_RenderOrder; // レンダリング順を決めるための配列	  // -- 2025.3.15

    VECTOR3 m_vMin; // モデルの大きさを測るための最小値  // -- 2021.2.4
    VECTOR3 m_vMax; // モデルの大きさを測るための最大値  // -- 2021.2.4

public:
    // メソッド
    /// CFbxMesh を初期化する
    CFbxMesh();
    /// CFbxMesh を初期化する
    /// @param pFbxMeshCtrl pFbxMeshCtrl に渡す値
    CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl); // -- 2021.2.4
    /// CFbxMesh を初期化する
    /// @param pFbxMeshCtrl pFbxMeshCtrl に渡す値
    /// @param FName 名前
    CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl, const TCHAR* FName);
    /// CFbxMesh の終了処理を行う
    ~CFbxMesh();

    /// D3 D を破棄する
    void DestroyD3D();

    /// 読み込む
    /// @param FName 名前
    /// @return 成功または条件を満たす場合 true
    bool Load(const TCHAR* FName);

    /// Animation を読み込む
    /// @param id id に渡す値
    /// @param FName 名前
    /// @param loopFlag ループ再生するか
    /// @param RAType RAType に渡す値
    /// @return 成功または条件を満たす場合 true
    bool LoadAnimation(int id, const TCHAR* FName, bool loopFlag, const ROOTANIMTYPE& RAType = eRootAnimNone);

    /// Static Center Pos を取得する
    /// @param vertex 使用するテクスチャ
    /// @param Num Num に渡す値
    /// @return 3次元ベクトル
    VECTOR3 GetStaticCenterPos(const StaticVertex* vertex, const DWORD& Num);
    /// Static Vertex Layout を切り替える
    /// @param vertices vertices に渡す値
    /// @param indices indices に渡す値
    /// @param IndicesNum IndicesNum に渡す値
    /// @param verticesNormal verticesNormal に渡す値
    /// @return 成功または条件を満たす場合 true
    bool ChangeStaticVertexLayout(const StaticVertex* vertices, const DWORD* indices, const DWORD& IndicesNum,
                                  StaticVertexNormal* verticesNormal);
    /// Static VIBuffer を設定する
    /// @param mi mi に渡す値
    /// @param vertices vertices に渡す値
    /// @param indices indices に渡す値
    /// @return 処理結果
    HRESULT SetStaticVIBuffer(const DWORD& mi, const StaticVertexNormal* vertices, const DWORD* indices);

    /// Skin Center Pos を取得する
    /// @param vertex 使用するテクスチャ
    /// @param Num Num に渡す値
    /// @return 3次元ベクトル
    VECTOR3 GetSkinCenterPos(const SkinVertex* vertex, const DWORD& Num);
    /// Skin Vertex Layout を切り替える
    /// @param vertices vertices に渡す値
    /// @param indices indices に渡す値
    /// @param IndicesNum IndicesNum に渡す値
    /// @param verticesNormal verticesNormal に渡す値
    /// @return 成功または条件を満たす場合 true
    bool ChangeSkinVertexLayout(const SkinVertex* vertices, const DWORD* indices, const DWORD& IndicesNum,
                                SkinVertexNormal* verticesNormal);
    /// Skin VIBuffer を設定する
    /// @param mi mi に渡す値
    /// @param vertices vertices に渡す値
    /// @param indices indices に渡す値
    /// @return 処理結果
    HRESULT SetSkinVIBuffer(const DWORD& mi, const SkinVertexNormal* vertices, const DWORD* indices);

    /// Render Idx Array を設定する
    /// @param mWorld mWorld に渡す値
    /// @param vEye vEye に渡す値
    void SetRenderIdxArray(const MATRIX4X4& mWorld, const VECTOR3& vEye);
    /// Tangent Sub を計算する
    /// @param v1 v1 に渡す値
    /// @param v2 v2 に渡す値
    /// @param v3 v3 に渡す値
    /// @param uv1 uv1 に渡す値
    /// @param uv2 uv2 に渡す値
    /// @param uv3 uv3 に渡す値
    /// @param tangent tangent に渡す値
    /// @param Binormal Binormal に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CalcTangentSub(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3,
                        const VECTOR2& uv1, const VECTOR2& uv2, const VECTOR2& uv3, VECTOR3& tangent,
                        VECTOR3& Binormal);
    /// Texture を設定する
    /// @param ti ti に渡す値
    /// @param TexName 名前
    /// @return 処理結果
    HRESULT SetTexture(const DWORD& ti, const TCHAR* TexName);
    /// Material を設定する
    /// @param ti ti に渡す値
    /// @param TexName 名前
    /// @return 成功または条件を満たす場合 true
    bool SetMaterial(const DWORD& ti, const TCHAR* TexName); // -- 2020.12.15

    /// Conv Indices Data の処理を行う
    /// @param mi mi に渡す値
    void ConvIndicesData(int mi); // -- 2024.3.23


    /// Root Anim Type を取得する
    /// @param animID animID に渡す値
    /// @return 処理結果の数値
    int GetRootAnimType(int animID) { return m_RootAnimType[animID]; } // -- 2024.9.5

    /// Root Anim Matrices を取得する
    /// @param animStatus animStatus に渡す値
    /// @param UpFrame UpFrame に渡す値
    /// @return 行列
    MATRIX4X4 GetRootAnimMatrices(Animator* animStatus, const int& UpFrame = 0); // -- 2024.3.14

    /// Root Anim Up Matrices を取得する
    /// @param animStatus animStatus に渡す値
    /// @param UpFrame UpFrame に渡す値
    /// @param StartFrameUp StartFrameUp に渡す値
    /// @return 行列
    MATRIX4X4 GetRootAnimUpMatrices(Animator* animStatus, const int& UpFrame = 0, const int& StartFrameUp = 1);
    // -- 2024.3.14

    /// Root Anim And Shader Matrix を作成する
    /// @param animNum animNum に渡す値
    /// @param RAType RAType に渡す値
    void MakeRootAnimAndShaderMatrix(const int& animNum, ROOTANIMTYPE RAType);
    // ルートボーンアニメーションとシェーダーマトリックスの作成       // -- 2020.12.15 -- 3

    // フレーム情報の取得
    /// End Frame を取得する
    /// @param animID animID に渡す値
    /// @return 処理結果の数値
    int GetEndFrame(const int& animID); // endFrame-startFrameを取得 

    /// Frame Matrices を取得する
    /// @param animStatus animStatus に渡す値
    /// @param nBone nBone に渡す値
    /// @param nMesh nMesh に渡す値
    /// @return 行列
    MATRIX4X4 GetFrameMatrices(Animator* animStatus, const DWORD& nBone, const DWORD& nMesh = 0); // -- 2024.9.5
    /// Frame Matrices を取得する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param nBone nBone に渡す値
    /// @param nMesh nMesh に渡す値
    /// @return 行列
    MATRIX4X4 GetFrameMatrices(Animator* animStatus, const MATRIX4X4& mWorld, const DWORD& nBone,
                               const DWORD& nMesh = 0); // -- 2024.9.5
    /// Mix Bone Matrix を取得する
    /// @param animStatus animStatus に渡す値
    /// @param nBone nBone に渡す値
    /// @param nMesh nMesh に渡す値
    /// @return 行列
    MATRIX4X4 GetMixBoneMatrix(Animator* animStatus, const DWORD& nBone, const DWORD& nMesh);

    /// 描画する
    /// @param mWorld mWorld に渡す値
    void Render(const MATRIX4X4& mWorld);
    /// Displace を描画する
    /// @param mWorld mWorld に渡す値
    void RenderDisplace(const MATRIX4X4& mWorld);

    /// 描画する
 	/// @param animStatus animStatus に渡す値
 	/// @param mWorld mWorld に渡す値
    void Render(Animator* animStatus, const MATRIX4X4& mWorld);
    /// Displace を描画する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    void RenderDisplace(Animator* animStatus, const MATRIX4X4& mWorld);

    // スタティックメッシュ	レンダリング
    /// 描画する
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight,
                const VECTOR3& vEye);
    /// Displace を描画する
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void RenderDisplace(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight,
                        const VECTOR3& vEye);
    /// Instance を取得する
    /// @return 対象のポインタ
    CFbxMesh* GetInstance();

private:
    /// Static を描画する
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void RenderStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight,
                      const VECTOR3& vEye);
    /// Displace Static を描画する
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void RenderDisplaceStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                              const VECTOR3& vLight, const VECTOR3& vEye);

public:
    // スキンメッシュ   レンダリング
    /// 描画する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void Render(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                const VECTOR3& vLight, const VECTOR3& vEye);
    /// Skin を描画する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void RenderSkin(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                    const VECTOR3& vLight, const VECTOR3& vEye);

    /// Bone Matrix を作成する
    /// @param animStatus animStatus に渡す値
    /// @param mat mat に渡す値
    /// @param _mesh _mesh に渡す値
    void MakeBoneMatrix(Animator* animStatus, MATRIX4X4* mat, const CFbxMeshArray& _mesh);

private:
    /// Displace Skin を描画する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void RenderDisplaceSkin(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView,
                            const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
    /// Displace を描画する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void RenderDisplace(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                        const VECTOR3& vLight, const VECTOR3& vEye);

    /// 描画する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void Draw(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj,
              const VECTOR3& vLight, const VECTOR3& vEye);
    /// Displace を描画する
    /// @param animStatus animStatus に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void DrawDisplace(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj,
                      const VECTOR3& vLight, const VECTOR3& vEye);

public:
    // シェーダー関連
    /// Shader Texture を設定する
    /// @param SlotNo SlotNo に渡す値
    /// @param ppTexture 使用するテクスチャ
    void SetShaderTexture(const TCHAR ProfileName[], int SlotNo, ID3D11ShaderResourceView** ppTexture);
    /// Shader Mat Color を設定する
    /// @param SlotNo SlotNo に渡す値
    /// @param i i に渡す値
    void SetShaderMatColor(const TCHAR ProfileName[], int SlotNo, int i); // -- 2020.12.15

    // ライトの強度を設定/取得
    /// Light Intensity を設定する
    /// @param r r に渡す値
    /// @param g g に渡す値
    /// @param b b に渡す値
    /// @param a a に渡す値
    void SetLightIntensity(float r, float g, float b, float a = 1.0f) { m_vDiffuse = VECTOR4(r, g, b, a); }
    /// Light Intensity を設定する
    /// @param intensity intensity に渡す値
    void SetLightIntensity(const VECTOR4& intensity) { m_vDiffuse = intensity; }
    /// Light Intensity を取得する
    /// @return 4次元ベクトル
    VECTOR4 GetLightIntensity() const { return m_vDiffuse; }
};

//
// Fbxメッシュコントロールクラス            // -- 2021.2.4
// 
// メッシュの総合的な管理を行うクラス
// 主にテクスチャの管理を行う。
// 複数のメッシュ間で同一のテクスチャが使われている場合、重複する読み込みを排除する
//
class CFbxMeshCtrl
{
public:
    CDirect3D* m_pD3D; // Direct3D11
    CShader* m_pShader;

    std::list<TextureList> m_TexList; // テクスチャーのリスト

public:
    /// Texture List を設定する
    /// @return 対象のポインタ
    ID3D11ShaderResourceView* SetTextureList(const TCHAR FName[]);
    /// Texture List を削除する
    void DeleteTextureList(ID3D11ShaderResourceView*);

    /// CFbxMeshCtrl を初期化する
    /// @param pShader pShader に渡す値
    CFbxMeshCtrl(CShader* pShader);
    /// CFbxMeshCtrl の終了処理を行う
    ~CFbxMeshCtrl();
};
