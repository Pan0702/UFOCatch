//------------------------------------------------------------------------
//    メッシュ接触判定用のライブラリ
//	  											ver 4.0         2025.1.3
//	  Collision.h								
//------------------------------------------------------------------------
#pragma once

// 最適化の中止
//#pragma optimize("", off)

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "Direct3D.h"
#include "Shader.h"
#include "FbxMesh.h"


// ---------------------------------------------------------
//
// マクロ定義
//
// ---------------------------------------------------------
// 壁と床との区別をするために使用。面法線のＹ方向の値である
// 0.5fのときは法線角度は30ﾟであるので、面の角度は60°となる
// 0.6fのときは法線角度は約37ﾟであるので、面の角度は約53°となる。
#define  GROUND  0.6f

// 重力の加速度
// Collisionの中では使用していない。ゲーム中で使用している
//#define  GRAVITY  -9.8f

// AddAngle関数の戻り値である角度（ラジアン）の限界値
// 理論値は、2.0であるが誤差を考慮して>=1.99f～1.95f程度とする
#define  ADDANGLELIMIT  1.99f

// HightCheck関数で使用する移動先床を判定する高さの上限値
// 上り坂を考慮して、初期値は1.0であるが、移動やジャンプスピードと勾配を考慮して1.0～10.0程度とする
#define  UPHILLLIMIT  1.0f

// CheckFloorMove関数で使用する移動先床に追従する高さの下限値
// 移動後点の床がこの高さより低いと、空中に浮かぶようになる（落下中）
#define  LOWFLOORLIMIT  -0.2f

// HightCheck関数で使用するキャラクターの足下の床からの高さ
// この値だけ、足下と床の間に隙間ができる
// この値を小さくしすぎると床と壁の境目から抜け落ちる可能性が高くなる
#define  UNDERFOOTLIMIT  0.05f

// メッシュ接触判定配列最大要素数
#define  MCKTBL_MAX     100

// 複数分割度マップ最大分割数
#define  MESHCKTBL_FACE_MAX  8
//#define  MESHCKTBL_FACE_MAX  1

// 複数分割度マップの最小サイズ
#define  MESHCKTBL_LOWLIMIT  3.0f

// コリジョン関数の戻り値
enum CollRet
{
	clError = -1,  // エラー
	clMove = 1,	   // 平行移動中
	clLand = 2,	   // 着地
	clFall = 3	   // 落下中
};

// ---------------------------------------------------------
//
// AABB（軸並行境界ボックス：Axis - Aligned Bounding Box）クラス
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
	DWORD				dwIdx[3];	// ３角形ポリゴンインデックス
	VECTOR3				vNormal;	// ３角形ポリゴン法線
	CAABB				AABB;		// 軸平行バウンディングボックス
};

// ---------------------------------------------------------
// コリジョンメッシュ
// ---------------------------------------------------------
struct ColMesh
{
	ColFace*			pFace;
	VECTOR3*			pVert;
	int					nNumFace;
	int					nNumVert;

	ColMesh()
	{
		pFace		= nullptr;
		pVert		= nullptr;
		nNumFace	= 0;
		nNumVert	= 0;
	}
};
// ---------------------------------------------------------
// 複数分割度マップ
// ---------------------------------------------------------
//
// １ポリゴンのアドレス格納用
// リスト構造となっている
//
struct ChkFace
{
	ColFace*			pFace;
	ChkFace*			pNext;
	ChkFace()
	{
		ZeroMemory(this, sizeof(struct ChkFace));
	}
};
//
// 同一分割度のポリゴンリスト格納用
//
struct ChkBlk
{
	ChkFace**			ppChkFace;  // dwNumX*dwNumY*dwNumZのポインタ配列
	VECTOR3				vBlksize;
	DWORD				dwNumX, dwNumY, dwNumZ;
	ChkBlk()
	{
		ZeroMemory(this, sizeof(struct ChkBlk));
	}
};
//
// 複数分割度マップ格納用
//
struct ChkColMesh
{
	ChkBlk				ChkBlkArray[MESHCKTBL_FACE_MAX];
	VECTOR3				vMin;
	VECTOR3				vMax;							// -- 2020.12.3
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
	CDirect3D*			m_pD3D;
	CShader*			m_pShader;
	CFbxMeshCtrl*		m_pFbxMeshCtrl;			// -- 2021.2.4

	// 高さ判定用の変数
	struct ColFace*		m_pIndex;
	VECTOR3				m_vNormalH;
	float				m_fHeight;
	VECTOR3				m_vVertexH[3];

	// メッシュ接触判定配列
	// (vectorクラスは遅いため配列を使用する)
	ColMesh				m_ColArray[MCKTBL_MAX];
	int					m_nNum;			// 配列の要素数

	// 複数分割度マップ
	ChkColMesh			m_ChkColMesh[MCKTBL_MAX];

	// コリジョン移動用
	bool				m_bMoveFlag;	// 移動するかどうか 移動の時 真
	MATRIX4X4			m_mWorldOld;	// 移動マトリックス（現在位置の一つ前）
	MATRIX4X4			m_mWorld;		// 移動マトリックスの現在位置
	MATRIX4X4			m_mWorldInv;	// 移動マトリックスの現在位置の逆マトリックス

public:
	CCollision();		   // -- 2024.3.13
	CCollision(CFbxMeshCtrl* pFbxMeshCtrl);   // -- 2021.2.4

	~CCollision();

	/// <summary>
	/// FbxMeshファイルから、コリジョン判定データを作成する
	/// 判定データは座標原点に設定される
	/// </summary>
	/// <param name="">モデルデータのパス</param>
	/// <returns>true:正常  false:ファイルが見つからない</returns>
	bool	AddFbxLoad( const TCHAR*);

	/// <summary>
	/// FbxMeshファイルから、コリジョン判定データを作成する
	/// 判定データを設定する座標を指定する
	/// </summary>
	/// <param name="">モデルデータのパス</param>
	/// <param name="vOffset">設定する座標</param>
	/// <returns>true:正常  false:ファイルが見つからない</returns>
	bool	AddFbxLoad( const TCHAR*, const VECTOR3& vOffset);

	/// <summary>
	/// FbxMeshファイルから、コリジョン判定データを作成する
	/// 判定データを設定するワールドマトリックスを指定する
	/// </summary>
	/// <param name="">モデルデータのパス</param>
	/// <param name="mOffset">設定するマトリックス</param>
	/// <returns>true:正常  false:ファイルが見つからない</returns>
	bool	AddFbxLoad( const TCHAR*, const MATRIX4X4& mOffset);

	/// <summary>
	/// FbxMeshデータから、コリジョン判定データを作成する
	/// 判定データは座標原点に設定される
	/// </summary>
	/// <param name="">メッシュデーター</param>
	/// <returns>true:正常  false:ファイルが見つからない</returns>
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh);

	/// <summary>
	/// FbxMeshデータから、コリジョン判定データを作成する
	/// 判定データを設定する座標を指定する
	/// </summary>
	/// <param name="">メッシュデーター</param>
	/// <param name="vOffset">設定する座標</param>
	/// <returns>true:正常  false:ファイルが見つからない</returns>
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh, const VECTOR3& vOffset);

 	/// <summary>
	/// FbxMeshデータから、コリジョン判定データを作成する
	/// 判定データを設定するワールドマトリックスを指定する
	/// </summary>
	/// <param name="">メッシュデーター</param>
	/// <param name="vOffset">設定するワールドマトリックス</param>
	/// <returns>true:正常  false:ファイルが見つからない</returns>
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh, const MATRIX4X4& mOffset);

	/// <summary>
	/// 線分との当たり判定を行う
	/// 始点から一番近いポリゴンの当たり判定情報を返します
	/// ポリゴンの表面のみ判定し、裏面は判定しません
	/// </summary>
	/// <param name="startIn">線分の始点</param>
	/// <param name="endIn">線分の終点</param>
	/// <param name="vHit">当たった位置を入れる場所(Out)</param>
	/// <param name="vNormal">当たった位置の法線を入れる場所(Out)</param>
	/// <returns>当たっていればtrue</returns>
	bool    IsCollisionLay(const VECTOR3& startIn, const VECTOR3& endIn, VECTOR3& vHit, VECTOR3& vNormal);

	/// <summary>
	/// 球体との当たり判定を行う
	/// 移動開始から一番近いポリゴンの当たり判定情報を返します
	/// </summary>
	/// <param name="startIn">移動開始点</param>
	/// <param name="endIn">移動終了点</param>
	/// <param name="fRadius">半径</param>
	/// <param name="vHit">当たった位置を入れる場所(Out)</param>
	/// <param name="vNormal">当たった位置の法線を入れる場所(Out)</param>
	/// <returns>当たっていればtrue</returns>
	bool    IsCollisionSphere(const VECTOR3& startIn, const VECTOR3& endIn, const float& fRadius, VECTOR3& vHit, VECTOR3& vNormal);

	/// <summary>
	/// 球体の移動線分との当たり判定を行う
	/// 移動開始点から一番近いポリゴンで当たり判定を行います
	/// 当たっていた場合、球体がポリゴンの外に出るように移動終了点positionの値が変更されます
	/// </summary>
	/// <param name="positionOld">移動前点</param>
	/// <param name="position">移動後点(In/Out)</param>
	/// <param name="fRadius">半径</param>
	/// <returns>当たっていればtrue</returns>
	bool    IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

	/// <summary>
	/// 球体の移動線分との当たり判定を行う
	/// 移動開始点から一番近いポリゴンの当たり判定情報を返します
	/// 当たっていた場合、球体がポリゴンの外に出るように移動終了点positionの値が変更されます
	/// </summary>
	/// <param name="positionOld">移動前点</param>
	/// <param name="position">移動後点(In/Out)</param>
	/// <param name="vHit">当たった位置を入れる場所(Out)</param>
	/// <param name="vNormal">当たった位置の法線を入れる場所(Out)</param>
	/// <param name="fRadius">半径</param>
	/// <returns>当たっていればtrue</returns>
	bool    IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);

	/// <summary>
	/// 球体の移動線分との重力を加味した当たり判定を行う
	/// 移動開始点から一番近いポリゴンで当たり判定を行います
	/// 当たっていた場合、球体がポリゴンの外に出るように移動終了点positionの値が変更されます
	/// </summary>
	/// <param name="positionOld">移動前点</param>
	/// <param name="position">移動後点(In/Out)</param>
	/// <param name="fRadius">半径</param>
	/// <returns>コリジョン判定の結果</returns>
	CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

	/// 球体の移動線分との重力を加味した当たり判定を行う
	/// 移動開始点から一番近いポリゴンで当たり判定を行います
	/// 当たっていた場合、球体がポリゴンの外に出るように移動終了点positionの値が変更されます
	/// </summary>
	/// <param name="positionOld">移動前点</param>
	/// <param name="position">移動後点(In/Out)</param>
	/// <param name="vHit">当たった位置を入れる場所(Out)</param>
	/// <param name="vNormal">当たった位置の法線を入れる場所(Out)</param>
	/// <param name="fRadius">半径</param>
	/// <returns>コリジョン判定の結果</returns>
	CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& pisition, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);

	/// <summary>
	/// 移動マップのマトリックスの初期化と設定、参照等
	/// (通常のマップには関係しない)
	/// </summary>
	/// <param name="">ワールドマトリックス</param>
	void	InitWorldMatrix(const MATRIX4X4&);
	void	SetWorldMatrix(const MATRIX4X4&);
	void	SaveWorldMatrix() { m_mWorldOld = m_mWorld; }
	MATRIX4X4 WorldMatrix(){ return m_mWorld;}

	/// <summary>
	/// マップフラグの参照
	/// </summary>
	/// <returns>移動マップのときtrue</returns>
	bool	MoveFlag() { return m_bMoveFlag; }

	void    GetChkAABB(VECTOR3& vMin, VECTOR3& vMax);

private:
	void	makeChkColMesh(const int& nNum, const VECTOR3& vMin, const VECTOR3& vMax);
	void	getChkArrayIdx(const int& nNum, const int& nNo, CAABB AABB, int nIdx[], int& nIMax);
	void	setChkArray(const int& nNum, const int& nNo, const int& nIdx, ColFace* pFace);
	void	clearAll(void);
	void	deleteAll(void);

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
