//------------------------------------------------------------------------
//    メッシュ衝突判定用のライブラリ
//                                   ver 4.0         2025.1.3
//    Collision.h                       
//------------------------------------------------------------------------
#pragma once

// 最適化の中止
//#pragma optimize("", off)

// ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "../Core/Graphics/Direct3D.h"
#include "../Framework/Shader.h"
#include "FbxMesh.h"


// ---------------------------------------------------------
//
// マクロ定義
//
// ---------------------------------------------------------
// 地面と壁の区別をするために使用。面法線のy成分の値である。
// 0.5fのときは法線角度が30度であるので、面の角度は60度となる。
// 0.6fのときは法線角度は約37度であるので、面の角度は約53度となる。
#define  GROUND  0.6f

// 重力の加速度
// Collisionの中では使用していない。ゲーム側で使用している。
//#define  GRAVITY  -9.8f

// AddAngle関数の戻り値である角度（ラジアン）の限界値
#define  ADDANGLELIMIT  1.99f

// HeightCheck関数で使用する移動先段差を判定する高さの上限値
// 経験値として、理論値は1.0であるが、移動座標（計算スピード）と誤差を考慮して1.0～10.0程度とする
#define  UPHILLLIMIT  1.0f

// CheckFloorMove関数で使用する移動先段差に追従する高さの下限値
// 移動前後の高低差が低いと、空中を歩き回るようになる（浮遊防止）
#define  LOWFLOORLIMIT  -0.2f

// HeightCheck関数で使用するキャラクターの足元の遊びの高さ
// この値により、地面と足の間に隙間ができる
// この値を大きくしすぎると地面と壁の境界から突き抜ける可能性があるため注意
#define  UNDERFOOTLIMIT  0.05f

// メッシュ衝突判定テーブル最大要素数
#define  MCKTBL_MAX     100

// 空間分割マップ最大分割数
#define  MESHCKTBL_FACE_MAX  8
//#define  MESHCKTBL_FACE_MAX  1

// 空間分割マップの最小サイズ
#define  MESHCKTBL_LOWLIMIT  3.0f

// コリジョン関数の戻り値
enum CollRet
{
    clError = -1, // エラー
    clMove = 1, // 歩行移動中
    clLand = 2, // 着地
    clFall = 3 // 落下中
};

// ---------------------------------------------------------
//
// AABB（軸並行境界ボックス：Axis-Aligned Bounding Box）クラス
//
// ---------------------------------------------------------
class CAABB
{
public:
    VECTOR3 m_vMax;
    VECTOR3 m_vMin;

public:
    CAABB() = default;

    CAABB(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3)
    {
        MakeAABB(v1, v2, v3);
    }

    CAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r = 0.0f)
    {
        MakeAABB(v1, v2, r);
    }

    /// AABB を作成する
    /// @param v1 v1 に渡す値
    /// @param v2 v2 に渡す値
    /// @param v3 v3 に渡す値
    void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3);
    /// AABB を作成する
    /// @param v1 v1 に渡す値
    /// @param v2 v2 に渡す値
    /// @param r r に渡す値
    void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r = 0.0f);
    /// Hitcheck を返す
    /// @param other other に渡す値
    /// @return 成功または条件を満たす場合 true
    bool Hitcheck(const CAABB& other);
    /// Hitcheck XZ を返す
    /// @param other other に渡す値
    /// @return 成功または条件を満たす場合 true
    bool HitcheckXZ(const CAABB& other);
    /// Vec Pos を取得する
    /// @param nIdx nIdx に渡す値
    /// @return 3次元ベクトル
    VECTOR3 GetVecPos(const int& nIdx);
};

// ---------------------------------------------------------
// コリジョンメッシュの1ポリゴン
// ---------------------------------------------------------
struct ColFace
{
    DWORD dwIdx[3]; // 三角形ポリゴンインデックス
    VECTOR3 vNormal; // 三角形ポリゴン法線
    CAABB AABB; // 軸並行バウンディングボックス
};

// ---------------------------------------------------------
// コリジョンメッシュ
// ---------------------------------------------------------
struct ColMesh
{
    ColFace* pFace;
    VECTOR3* pVert;
    int nNumFace;
    int nNumVert;

    /// ColMesh を初期化する
    ColMesh()
    {
        pFace = nullptr;
        pVert = nullptr;
        nNumFace = 0;
        nNumVert = 0;
    }
};

// ---------------------------------------------------------
// 空間分割マップ
// ---------------------------------------------------------
//
// 1ポリゴンのアドレス格納用
// リスト構造となっている
//
struct ChkFace
{
    ColFace* pFace;
    ChkFace* pNext;
    /// ChkFace を初期化する
    ChkFace()
    {
        ZeroMemory(this, sizeof(struct ChkFace));
    }
};

//
// 空間分割後のポリゴンリスト格納用
//
struct ChkBlk
{
    ChkFace** ppChkFace; // dwNumX*dwNumY*dwNumZのポインタ配列
    VECTOR3 vBlksize;
    DWORD dwNumX, dwNumY, dwNumZ;
    /// ChkBlk を初期化する
    ChkBlk()
    {
        ZeroMemory(this, sizeof(struct ChkBlk));
    }
};

//
// 空間分割マップ格納用
//
struct ChkColMesh
{
    ChkBlk ChkBlkArray[MESHCKTBL_FACE_MAX];
    VECTOR3 vMin;
    VECTOR3 vMax; // -- 2020.12.3
    /// ChkColMesh を初期化する
    ChkColMesh()
    {
        ZeroMemory(this, sizeof(struct ChkColMesh));
    }
};

/// <summary>汎用ユーティリティで使う Fbx Mesh Ctrl の情報と処理をまとめる型</summary>
class CFbxMeshCtrl;
/// <summary>汎用ユーティリティで使う Fbx Mesh の情報と処理をまとめる型</summary>
class CFbxMesh;
// ---------------------------------------------------------
//
// コリジョンクラス
//
// ---------------------------------------------------------
class CCollision
{
private:
    CDirect3D* m_pD3D;
    CShader* m_pShader;
    CFbxMeshCtrl* m_pFbxMeshCtrl; // -- 2021.2.4

    // 衝突判定用の変数
    struct ColFace* m_pIndex;
    VECTOR3 m_vNormalH;
    float m_fHeight;
    VECTOR3 m_vVertexH[3];

    // メッシュ衝突判定用配列
    // (vectorクラスは遅いため配列を使用する)
    ColMesh m_ColArray[MCKTBL_MAX];
    int m_nNum; // 配列の要素数

    // 空間分割マップ
    ChkColMesh m_ChkColMesh[MCKTBL_MAX];

    // コリジョン移動用
    bool m_bMoveFlag; // 移動するかどうか (移動の主導)
    MATRIX4X4 m_mWorldOld; // 移動マトリックス（現在位置の一つ前）
    MATRIX4X4 m_mWorld; // 移動マトリックスの現在位置
    MATRIX4X4 m_mWorldInv; // 移動マトリックスの現在位置の逆マトリックス

public:
    /// CCollision を初期化する
    CCollision(); // -- 2024.3.13
    /// CCollision を初期化する
    /// @param pFbxMeshCtrl pFbxMeshCtrl に渡す値
    CCollision(CFbxMeshCtrl* pFbxMeshCtrl); // -- 2021.2.4

    /// CCollision の終了処理を行う
    ~CCollision();

    /// Fbx Load を追加する
    /// @return 成功または条件を満たす場合 true
    bool AddFbxLoad(const TCHAR*);

    /// Fbx Load を追加する
    /// @param vOffset vOffset に渡す値
    /// @return 成功または条件を満たす場合 true
    bool AddFbxLoad(const TCHAR*, const VECTOR3& vOffset);

    /// Fbx Load を追加する
    /// @param mOffset mOffset に渡す値
    /// @return 成功または条件を満たす場合 true
    bool AddFbxLoad(const TCHAR*, const MATRIX4X4& mOffset);

    /// Fbx Load を追加する
    /// @param pFbxMesh pFbxMesh に渡す値
    /// @return 成功または条件を満たす場合 true
    bool AddFbxLoad(const CFbxMesh* pFbxMesh);

    /// Fbx Load を追加する
    /// @param pFbxMesh pFbxMesh に渡す値
    /// @param vOffset vOffset に渡す値
    /// @return 成功または条件を満たす場合 true
    bool AddFbxLoad(const CFbxMesh* pFbxMesh, const VECTOR3& vOffset);

    /// Fbx Load を追加する
    /// @param pFbxMesh pFbxMesh に渡す値
    /// @param mOffset mOffset に渡す値
    /// @return 成功または条件を満たす場合 true
    bool AddFbxLoad(const CFbxMesh* pFbxMesh, const MATRIX4X4& mOffset);

    /// Collision Lay を判定する
    /// @param startIn startIn に渡す値
    /// @param endIn endIn に渡す値
    /// @param vHit vHit に渡す値
    /// @param vNormal vNormal に渡す値
    /// @return 成功または条件を満たす場合 true
    bool IsCollisionLay(const VECTOR3& startIn, const VECTOR3& endIn, VECTOR3& vHit, VECTOR3& vNormal);

    /// Collision Sphere を判定する
    /// @param startIn startIn に渡す値
    /// @param endIn endIn に渡す値
    /// @param fRadius 半径
    /// @param vHit vHit に渡す値
    /// @param vNormal vNormal に渡す値
    /// @return 成功または条件を満たす場合 true
    bool IsCollisionSphere(const VECTOR3& startIn, const VECTOR3& endIn, const float& fRadius, VECTOR3& vHit,
                           VECTOR3& vNormal);

    /// Collision Move を判定する
    /// @param positionOld 座標
    /// @param position 座標
    /// @param fRadius 半径
    /// @return 成功または条件を満たす場合 true
    bool IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

    /// Collision Move を判定する
    /// @param positionOld 座標
    /// @param position 座標
    /// @param vHit vHit に渡す値
    /// @param vNormal vNormal に渡す値
    /// @param fRadius 半径
    /// @return 成功または条件を満たす場合 true
    bool IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal,
                         float fRadius = 0.2f);

    /// Collision Move Gravity を判定する
    /// @param positionOld 座標
    /// @param position 座標
    /// @param fRadius 半径
    /// @return 処理結果
    CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

    /// Collision Move Gravity を判定する
    /// @param positionOld 座標
    /// @param position 座標
    /// @param vHit vHit に渡す値
    /// @param vNormal vNormal に渡す値
    /// @param fRadius 半径
    /// @return 処理結果
    CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal,
                                   float fRadius = 0.2f);

    /// World Matrix を初期化する
    void InitWorldMatrix(const MATRIX4X4&);
    /// World Matrix を設定する
    void SetWorldMatrix(const MATRIX4X4&);
    /// World Matrix を保存する
    void SaveWorldMatrix() { m_mWorldOld = m_mWorld; }
    /// World Matrix を返す
    /// @return 行列
    MATRIX4X4 WorldMatrix() { return m_mWorld; }

    /// Flag を移動する
    /// @return 成功または条件を満たす場合 true
    bool MoveFlag() { return m_bMoveFlag; }

    /// Chk AABB を取得する
    /// @param vMin vMin に渡す値
    /// @param vMax vMax に渡す値
    void GetChkAABB(VECTOR3& vMin, VECTOR3& vMax);

private:
    /// make Chk Col Mesh の処理を行う
    /// @param nNum nNum に渡す値
    /// @param vMin vMin に渡す値
    /// @param vMax vMax に渡す値
    void makeChkColMesh(const int& nNum, const VECTOR3& vMin, const VECTOR3& vMax);
    /// get Chk Array Idx の処理を行う
    /// @param nNum nNum に渡す値
    /// @param nNo nNo に渡す値
    /// @param AABB AABB に渡す値
    /// @param nIMax nIMax に渡す値
    void getChkArrayIdx(const int& nNum, const int& nNo, CAABB AABB, int nIdx[], int& nIMax);
    /// set Chk Array の処理を行う
    /// @param nNum nNum に渡す値
    /// @param nNo nNo に渡す値
    /// @param nIdx nIdx に渡す値
    /// @param pFace pFace に渡す値
    void setChkArray(const int& nNum, const int& nNo, const int& nIdx, ColFace* pFace);
    /// clear All の処理を行う
    void clearAll(void);
    /// delete All の処理を行う
    void deleteAll(void);

    /// get Mesh Limit の処理を行う
    /// @param nNum nNum に渡す値
    /// @param nNo nNo に渡す値
    /// @param vNow vNow に渡す値
    /// @param vOld vOld に渡す値
    /// @param fRadius 半径
    /// @param nStatrX nStatrX に渡す値
    /// @param nEndX nEndX に渡す値
    /// @param nStatrY nStatrY に渡す値
    /// @param nEndY nEndY に渡す値
    /// @param nStatrZ nStatrZ に渡す値
    /// @param nEndZ nEndZ に渡す値
    void getMeshLimit(const int& nNum, const int& nNo, const VECTOR3& vNow, const VECTOR3& vOld, const float& fRadius,
                      int& nStatrX, int& nEndX, int& nStatrY, int& nEndY, int& nStatrZ, int& nEndZ);
    /// check Wall Move を返す
    /// @param positionOld 座標
    /// @param position 座標
    /// @param vHit vHit に渡す値
    /// @param vNormal vNormal に渡す値
    /// @param fRadius 半径
    /// @return 処理結果の数値
    int checkWallMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius);
    /// init Height Check の処理を行う
    void initHeightCheck();
    /// check Height を返す
    /// @param positionOld 座標
    /// @param position 座標
    /// @param fObjheight 高さ
    /// @return 成功または条件を満たす場合 true
    bool checkHeight(const VECTOR3& positionOld, VECTOR3& position, const float fObjheight);
    /// check Floor Move を返す
    /// @param positionOld 座標
    /// @param position 座標
    /// @return 処理結果
    CollRet checkFloorMove(const VECTOR3& positionOld, VECTOR3& position);
    /// check Collision Move を返す
    /// @param positionOld 座標
    /// @param position 座標
    /// @param vHit vHit に渡す値
    /// @param vNormal vNormal に渡す値
    /// @param fRadius 半径
    /// @return 処理結果の数値
    int checkCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal,
                           float fRadius); // -- 2024.9.10
    /// get Dist Normal の処理を行う
    /// @param vNow vNow に渡す値
    /// @param vOld vOld に渡す値
    /// @param vFaceNorm vFaceNorm に渡す値
    /// @param fNowDist fNowDist に渡す値
    /// @param fOldDist fOldDist に渡す値
    /// @param fLayDist fLayDist に渡す値
    void getDistNormal(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm,
                       float& fNowDist, float& fOldDist, float& fLayDist);
    /// check Lay を返す
    /// @param vNow vNow に渡す値
    /// @param vOld vOld に渡す値
    /// @param vFaceNorm vFaceNorm に渡す値
    /// @param fNowDist fNowDist に渡す値
    /// @param fOldDist fOldDist に渡す値
    /// @param fLayDist fLayDist に渡す値
    /// @param vHit vHit に渡す値
    /// @return 処理結果の数値
    int checkLay(const VECTOR3 [], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm,
                 const float& fNowDist, const float& fOldDist, const float& fLayDist, VECTOR3& vHit);
    /// check Near を返す
    /// @param vNow vNow に渡す値
    /// @param vFaceNorm vFaceNorm に渡す値
    /// @param fNowDist fNowDist に渡す値
    /// @param fRadius 半径
    /// @param vHit vHit に渡す値
    /// @return 処理結果の数値
    int checkNear(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vFaceNorm, const float& fNowDist,
                  float fRadius, VECTOR3& vHit);
};
