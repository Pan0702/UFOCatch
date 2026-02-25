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
// 理論値、2.0であるが誤差を考慮して >=1.99f? 1.95f程度とする
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
    clError = -1,  // エラー
    clMove = 1,    // 歩行移動中
    clLand = 2,    // 着地
    clFall = 3     // 落下中
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
    void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3);
    void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r = 0.0f);
    bool Hitcheck(const CAABB& other);
    bool HitcheckXZ(const CAABB& other);
    VECTOR3 GetVecPos(const int& nIdx);
};

// ---------------------------------------------------------
// コリジョンメッシュの1ポリゴン
// ---------------------------------------------------------
struct ColFace
{
    DWORD           dwIdx[3];  // 三角形ポリゴンインデックス
    VECTOR3         vNormal;   // 三角形ポリゴン法線
    CAABB           AABB;      // 軸並行バウンディングボックス
};

// ---------------------------------------------------------
// コリジョンメッシュ
// ---------------------------------------------------------
struct ColMesh
{
    ColFace* pFace;
    VECTOR3* pVert;
    int              nNumFace;
    int              nNumVert;

    ColMesh()
    {
       pFace     = nullptr;
       pVert     = nullptr;
       nNumFace   = 0;
       nNumVert   = 0;
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
    ChkFace** ppChkFace;  // dwNumX*dwNumY*dwNumZのポインタ配列
    VECTOR3          vBlksize;
    DWORD            dwNumX, dwNumY, dwNumZ;
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
    ChkBlk          ChkBlkArray[MESHCKTBL_FACE_MAX];
    VECTOR3         vMin;
    VECTOR3         vMax;                    // -- 2020.12.3
    ChkColMesh()
    {
       ZeroMemory(this, sizeof(struct ChkColMesh));
    }
};

class CFbxMeshCtrl;
class CFbxMesh;
// ---------------------------------------------------------
//
// コリジョンクラス
//
// ---------------------------------------------------------
class  CCollision
{
private:
    CDirect3D* m_pD3D;
    CShader* m_pShader;
    CFbxMeshCtrl* m_pFbxMeshCtrl;          // -- 2021.2.4

    // 衝突判定用の変数
    struct ColFace* m_pIndex;
    VECTOR3          m_vNormalH;
    float            m_fHeight;
    VECTOR3          m_vVertexH[3];

    // メッシュ衝突判定用配列
    // (vectorクラスは遅いため配列を使用する)
    ColMesh          m_ColArray[MCKTBL_MAX];
    int              m_nNum;          // 配列の要素数

    // 空間分割マップ
    ChkColMesh       m_ChkColMesh[MCKTBL_MAX];

    // コリジョン移動用
    bool             m_bMoveFlag;     // 移動するかどうか (移動の主導)
    MATRIX4X4        m_mWorldOld;     // 移動マトリックス（現在位置の一つ前）
    MATRIX4X4        m_mWorld;        // 移動マトリックスの現在位置
    MATRIX4X4        m_mWorldInv;     // 移動マトリックスの現在位置の逆マトリックス

public:
    CCollision();        // -- 2024.3.13
    CCollision(CFbxMeshCtrl* pFbxMeshCtrl);   // -- 2021.2.4

    ~CCollision();

    /// <summary>
    /// FbxMeshファイルから、コリジョン用の当たりデータを作成する
    /// 当たりデータは原点に設定される
    /// </summary>
    /// <param name="">モデルデータのパス</param>
    /// <returns>true:成功  false:ファイルが見つからない等</returns>
    bool   AddFbxLoad( const TCHAR*);

    /// <summary>
    /// FbxMeshファイルから、コリジョン用の当たりデータを作成する
    /// 当たりデータを設定する座標を指定する
    /// </summary>
    /// <param name="">モデルデータのパス</param>
    /// <param name="vOffset">設定する座標</param>
    /// <returns>true:成功  false:ファイルが見つからない等</returns>
    bool   AddFbxLoad( const TCHAR*, const VECTOR3& vOffset);

    /// <summary>
    /// FbxMeshファイルから、コリジョン用の当たりデータを作成する
    /// 当たりデータを設定するワールドマトリックスを指定する
    /// </summary>
    /// <param name="">モデルデータのパス</param>
    /// <param name="mOffset">設定するマトリックス</param>
    /// <returns>true:成功  false:ファイルが見つからない等</returns>
    bool   AddFbxLoad( const TCHAR*, const MATRIX4X4& mOffset);

    /// <summary>
    /// FbxMeshデータから、コリジョン用の当たりデータを作成する
    /// 当たりデータは原点に設定される
    /// </summary>
    /// <param name="">メッシュデータ</param>
    /// <returns>true:成功  false:ファイルが見つからない等</returns>
    bool   AddFbxLoad(const CFbxMesh* pFbxMesh);

    /// <summary>
    /// FbxMeshデータから、コリジョン用の当たりデータを作成する
    /// 当たりデータを設定する座標を指定する
    /// </summary>
    /// <param name="">メッシュデータ</param>
    /// <param name="vOffset">設定する座標</param>
    /// <returns>true:成功  false:ファイルが見つからない等</returns>
    bool   AddFbxLoad(const CFbxMesh* pFbxMesh, const VECTOR3& vOffset);

    /// <summary>
    /// FbxMeshデータから、コリジョン用の当たりデータを作成する
    /// 当たりデータを設定するワールドマトリックスを指定する
    /// </summary>
    /// <param name="">メッシュデータ</param>
    /// <param name="mOffset">設定するワールドマトリックス</param>
    /// <returns>true:成功  false:ファイルが見つからない等</returns>
    bool   AddFbxLoad(const CFbxMesh* pFbxMesh, const MATRIX4X4& mOffset);

    /// <summary>
    /// 線分との交差判定を行う
    /// 始点から一番近いポリゴンの交差情報を返します
    /// ポリゴンの表面のみ判定し、裏面は判定しません
    /// </summary>
    /// <param name="startIn">線分の始点</param>
    /// <param name="endIn">線分の終点</param>
    /// <param name="vHit">交差した座標を受け取る場所(Out)</param>
    /// <param name="vNormal">交差した場所の法線を受け取る場所(Out)</param>
    /// <returns>交差していたらtrue</returns>
    bool    IsCollisionLay(const VECTOR3& startIn, const VECTOR3& endIn, VECTOR3& vHit, VECTOR3& vNormal);

    /// <summary>
    /// 球との交差判定を行う
    /// 移動開始点から一番近いポリゴンの交差情報を返します
    /// </summary>
    /// <param name="startIn">移動開始点</param>
    /// <param name="endIn">移動終了点</param>
    /// <param name="fRadius">半径</param>
    /// <param name="vHit">交差した座標を受け取る場所(Out)</param>
    /// <param name="vNormal">交差した場所の法線を受け取る場所(Out)</param>
    /// <returns>交差していたらtrue</returns>
    bool    IsCollisionSphere(const VECTOR3& startIn, const VECTOR3& endIn, const float& fRadius, VECTOR3& vHit, VECTOR3& vNormal);

    /// <summary>
    /// 球の移動経路とポリゴンとの交差判定を行う
    /// 移動開始点から一番近いポリゴンで交差判定を行います
    /// 交差している場合、球がポリゴンの外に出るように移動終了点positionの値を変更します
    /// </summary>
    /// <param name="positionOld">移動前点</param>
    /// <param name="position">移動後点(In/Out)</param>
    /// <param name="fRadius">半径</param>
    /// <returns>交差していたらtrue</returns>
    bool    IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

    /// <summary>
    /// 球の移動経路とポリゴンとの交差判定を行う
    /// 移動開始点から一番近いポリゴンの交差情報を返します
    /// 交差している場合、球がポリゴンの外に出るように移動終了点positionの値を変更します
    /// </summary>
    /// <param name="positionOld">移動前点</param>
    /// <param name="position">移動後点(In/Out)</param>
    /// <param name="vHit">交差した座標を受け取る場所(Out)</param>
    /// <param name="vNormal">交差した場所の法線を受け取る場所(Out)</param>
    /// <param name="fRadius">半径</param>
    /// <returns>交差していたらtrue</returns>
    bool    IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);

    /// <summary>
    /// 球の移動経路と重力の影響を考慮した交差判定を行う
    /// 移動開始点から一番近いポリゴンで交差判定を行います
    /// 交差している場合、球がポリゴンの外に出るように移動終了点positionの値を変更します
    /// </summary>
    /// <param name="positionOld">移動前点</param>
    /// <param name="position">移動後点(In/Out)</param>
    /// <param name="fRadius">半径</param>
    /// <returns>コリジョン判定の結果</returns>
    CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

    /// <summary>
    /// 球の移動経路と重力の影響を考慮した交差判定を行う
    /// 移動開始点から一番近いポリゴンで交差判定を行います
    /// 交差している場合、球がポリゴンの外に出るように移動終了点positionの値を変更します
    /// </summary>
    /// <param name="positionOld">移動前点</param>
    /// <param name="position">移動後点(In/Out)</param>
    /// <param name="vHit">交差した座標を受け取る場所(Out)</param>
    /// <param name="vNormal">交差した場所の法線を受け取る場所(Out)</param>
    /// <param name="fRadius">半径</param>
    /// <returns>コリジョン判定の結果</returns>
    CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);

    /// <summary>
    /// 移動マップのマトリックスの初期化と設定、参照用
    /// (通常のマップには関係ない)
    /// </summary>
    /// <param name="">ワールドマトリックス</param>
    void   InitWorldMatrix(const MATRIX4X4&);
    void   SetWorldMatrix(const MATRIX4X4&);
    void   SaveWorldMatrix() { m_mWorldOld = m_mWorld; }
    MATRIX4X4 WorldMatrix(){ return m_mWorld;}

    /// <summary>
    /// マップフラグの参照
    /// </summary>
    /// <returns>移動マップのときtrue</returns>
    bool   MoveFlag() { return m_bMoveFlag; }

    void    GetChkAABB(VECTOR3& vMin, VECTOR3& vMax);

private:
    void   makeChkColMesh(const int& nNum, const VECTOR3& vMin, const VECTOR3& vMax);
    void   getChkArrayIdx(const int& nNum, const int& nNo, CAABB AABB, int nIdx[], int& nIMax);
    void   setChkArray(const int& nNum, const int& nNo, const int& nIdx, ColFace* pFace);
    void   clearAll(void);
    void   deleteAll(void);

    void    getMeshLimit(const int& nNum, const int& nNo, const VECTOR3& vNow, const VECTOR3& vOld, const float& fRadius,
                      int& nStatrX, int& nEndX, int& nStatrY, int& nEndY, int& nStatrZ, int& nEndZ);
    int     checkWallMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius); 
    void    initHeightCheck();
    bool    checkHeight(const VECTOR3& positionOld, VECTOR3& position, const float fObjheight);
    CollRet checkFloorMove(const VECTOR3& positionOld, VECTOR3& position);
    int     checkCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius);    // -- 2024.9.10
    void    getDistNormal(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, float& fNowDist, float& fOldDist, float& fLayDist);
    int     checkLay(const VECTOR3[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, const float& fNowDist, const float& fOldDist, const float& fLayDist, VECTOR3& vHit);
    int     checkNear(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vFaceNorm, const float& fNowDist, float fRadius, VECTOR3& vHit);

};