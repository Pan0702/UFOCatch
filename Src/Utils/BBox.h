//=============================================================================
//
//  バウンディングボックス                             ver 3.3      2024.3.23
//
//   ポリゴンの表面判定を変更。左回り表とする
//									                                 BBox.h
//=============================================================================

#pragma once

#include "MyMath.h"
#include "../Core/Graphics/Direct3D.h"
#include "../Framework/Shader.h"

// ---------------------------------------------------------
//
//
// ---------------------------------------------------------
struct BBOX_VERTEX
{
    VECTOR3 vPos;
    VECTOR3 vNorm;
    VECTOR2 vTex; // テクスチャ座標(使用していない)
};

//
class CDirect3D;
/// <summary>Direct3D用シェーダと定数バッファを管理するクラス</summary>
class CShader;

/// <summary>汎用ユーティリティで使う BBox の情報と処理をまとめる型</summary>
class CBBox
{
public:
    // Direct3D11
    CDirect3D* m_pD3D;
    CShader* m_pShader;

    // OBB
    VECTOR3 m_vMax;
    VECTOR3 m_vMin;
    VECTOR3 m_vAxisX;
    VECTOR3 m_vAxisY;
    VECTOR3 m_vAxisZ;
    FLOAT m_fLengthX; // ボックスの半径(辺の長さの1/2)
    FLOAT m_fLengthY; // ボックスの半径(辺の長さの1/2)
    FLOAT m_fLengthZ; // ボックスの半径(辺の長さの1/2)
    MATRIX4X4 m_mWorld;

    VECTOR4 m_vDiffuse; // ディフューズカラー  // -- 2020.1.24

    // メッシュ
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pIndexBuffer;

public:
    //
    /// CBBox を初期化する
    CBBox(); // -- 2024.3.23
    /// CBBox を初期化する
    /// @param vMin vMin に渡す値
    /// @param vMax vMax に渡す値
    CBBox(const VECTOR3& vMin, const VECTOR3& vMax); // -- 2024.3.23
    /// CBBox を初期化する
    /// @param pShader pShader に渡す値
    CBBox(CShader* pShader);
    /// CBBox を初期化する
    /// @param pShader pShader に渡す値
    /// @param vMin vMin に渡す値
    /// @param vMax vMax に渡す値
    CBBox(CShader* pShader, const VECTOR3& vMin, const VECTOR3& vMax);
    /// CBBox の終了処理を行う
    ~CBBox();

    /// BBox を初期化する
    /// @param vMin vMin に渡す値
    /// @param vMax vMax に渡す値
    void InitBBox(const VECTOR3& vMin, const VECTOR3& vMax);
    /// Mesh を初期化する
    /// @return 処理結果
    HRESULT InitMesh();
    /// Vertex Index を作成する
    /// @param pVertex 使用するテクスチャ
    /// @param pIndex インデックス
    void MakeVertexIndex(VECTOR3* pVertex, DWORD* pIndex);
    /// OBBCollision Detection を返す
    /// @param pOtherBBox pOtherBBox に渡す値
    /// @param vHit vHit に渡す値
    /// @param vNrm vNrm に渡す値
    /// @return 成功または条件を満たす場合 true
    bool OBBCollisionDetection(CBBox* pOtherBBox, VECTOR3* vHit, VECTOR3* vNrm);
    /// Compare Length を返す
    /// @param pBBoxA pBBoxA に渡す値
    /// @param pBBoxB pBBoxB に渡す値
    /// @param pvSeparate pvSeparate に渡す値
    /// @param pvDistance pvDistance に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CompareLength(const CBBox* pBBoxA, const CBBox* pBBoxB, const VECTOR3* pvSeparate, const VECTOR3* pvDistance);
    /// OBBCollision Lay を返す
    /// @param mLay mLay に渡す値
    /// @param mLayOld mLayOld に渡す値
    /// @param vHit vHit に渡す値
    /// @param vNrm vNrm に渡す値
    /// @return 成功または条件を満たす場合 true
    bool OBBCollisionLay(const MATRIX4X4& mLay, const MATRIX4X4& mLayOld, VECTOR3* vHit, VECTOR3* vNrm);
    /// OBBCollision Lay を返す
    /// @param vLay vLay に渡す値
    /// @param vLayOld vLayOld に渡す値
    /// @param vHit vHit に渡す値
    /// @param vNrm vNrm に渡す値
    /// @return 成功または条件を満たす場合 true
    bool OBBCollisionLay(const VECTOR3& vLay, const VECTOR3& vLayOld, VECTOR3* vHit, VECTOR3* vNrm);
    /// OBBCollision Tri を返す
    /// @param pTri pTri に渡す値
    /// @param vHit vHit に渡す値
    /// @return 成功または条件を満たす場合 true
    bool OBBCollisionTri(const VECTOR3* pTri, VECTOR3* vHit);
    /// OBBCollision Tri を返す
    /// @param pTri pTri に渡す値
    /// @param mWorld mWorld に渡す値
    /// @param vhit vhit に渡す値
    /// @return 成功または条件を満たす場合 true
    bool OBBCollisionTri(const VECTOR3* pTri, const MATRIX4X4& mWorld, VECTOR3* vhit);
    /// Len Seg On Separate Axis を返す
    /// @param Sep Sep に渡す値
    /// @param e1 e1 に渡す値
    /// @param e2 e2 に渡す値
    /// @param e3 e3 に渡す値
    /// @return 計算結果の値
    float LenSegOnSeparateAxis(const VECTOR3* Sep, const VECTOR3* e1, const VECTOR3* e2, const VECTOR3* e3);

    /// 描画する
    void Render(); // -- 2024.3.23
    /// 描画する
    /// @param mWorld mWorld に渡す値
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight,
                const VECTOR3& vEye); // -- 2021.2.4
    /// 描画する
    /// @param mView mView に渡す値
    /// @param mProj mProj に渡す値
    /// @param vLight vLight に渡す値
    /// @param vEye vEye に渡す値
    void Render(const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
};
