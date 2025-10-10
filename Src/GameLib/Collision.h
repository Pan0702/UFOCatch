//------------------------------------------------------------------------
//    ���b�V���ڐG����p�̃��C�u����
//	  											ver 4.0         2025.1.3
//	  Collision.h								
//------------------------------------------------------------------------
#pragma once

// �œK���̒��~
//#pragma optimize("", off)

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>

#include "../DirectX/Direct3D.h"
#include "../FrameWork/Shader.h"
#include "FbxMesh.h"


// ---------------------------------------------------------
//
// �}�N����`
//
// ---------------------------------------------------------
// �ǂƏ��Ƃ̋�ʂ����邽�߂Ɏg�p�B�ʖ@���̂x�����̒l�ł���
// 0.5f�̂Ƃ��͖@���p�x��30߂ł���̂ŁA�ʂ̊p�x��60���ƂȂ�
// 0.6f�̂Ƃ��͖@���p�x�͖�37߂ł���̂ŁA�ʂ̊p�x�͖�53���ƂȂ�B
#define  GROUND  0.6f

// �d�͂̉����x
// Collision�̒��ł͎g�p���Ă��Ȃ��B�Q�[�����Ŏg�p���Ă���
//#define  GRAVITY  -9.8f

// AddAngle�֐��̖߂�l�ł���p�x�i���W�A���j�̌��E�l
// ���_�l�́A2.0�ł��邪�덷���l������>=1.99f�`1.95f���x�Ƃ���
#define  ADDANGLELIMIT  1.99f

// HightCheck�֐��Ŏg�p����ړ��揰�𔻒肷�鍂���̏���l
// ������l�����āA�����l��1.0�ł��邪�A�ړ���W�����v�X�s�[�h�ƌ��z���l������1.0�`10.0���x�Ƃ���
#define  UPHILLLIMIT  1.0f

// CheckFloorMove�֐��Ŏg�p����ړ��揰�ɒǏ]���鍂���̉����l
// �ړ���_�̏������̍������Ⴂ�ƁA�󒆂ɕ����Ԃ悤�ɂȂ�i�������j
#define  LOWFLOORLIMIT  -0.2f

// HightCheck�֐��Ŏg�p����L�����N�^�[�̑����̏�����̍���
// ���̒l�����A�����Ə��̊ԂɌ��Ԃ��ł���
// ���̒l����������������Ə��ƕǂ̋��ڂ��甲��������\���������Ȃ�
#define  UNDERFOOTLIMIT  0.05f

// ���b�V���ڐG����z��ő�v�f��
#define  MCKTBL_MAX     100

// ���������x�}�b�v�ő啪����
#define  MESHCKTBL_FACE_MAX  8
//#define  MESHCKTBL_FACE_MAX  1

// ���������x�}�b�v�̍ŏ��T�C�Y
#define  MESHCKTBL_LOWLIMIT  3.0f

// �R���W�����֐��̖߂�l
enum CollRet
{
	clError = -1,  // �G���[
	clMove = 1,	   // ���s�ړ���
	clLand = 2,	   // ���n
	clFall = 3	   // ������
};

// ---------------------------------------------------------
//
// AABB�i�����s���E�{�b�N�X�FAxis - Aligned Bounding Box�j�N���X
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
// �R���W�������b�V����1�|���S��
// ---------------------------------------------------------
struct ColFace
{
	DWORD				dwIdx[3];	// �R�p�`�|���S���C���f�b�N�X
	VECTOR3				vNormal;	// �R�p�`�|���S���@��
	CAABB				AABB;		// �����s�o�E���f�B���O�{�b�N�X
};

// ---------------------------------------------------------
// �R���W�������b�V��
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
// ���������x�}�b�v
// ---------------------------------------------------------
//
// �P�|���S���̃A�h���X�i�[�p
// ���X�g�\���ƂȂ��Ă���
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
// ���ꕪ���x�̃|���S�����X�g�i�[�p
//
struct ChkBlk
{
	ChkFace**			ppChkFace;  // dwNumX*dwNumY*dwNumZ�̃|�C���^�z��
	VECTOR3				vBlksize;
	DWORD				dwNumX, dwNumY, dwNumZ;
	ChkBlk()
	{
		ZeroMemory(this, sizeof(struct ChkBlk));
	}
};
//
// ���������x�}�b�v�i�[�p
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
// �R���W�����N���X
//
// ---------------------------------------------------------
class  CCollision
{
private:
	CDirect3D*			m_pD3D;
	CShader*			m_pShader;
	CFbxMeshCtrl*		m_pFbxMeshCtrl;			// -- 2021.2.4

	// ��������p�̕ϐ�
	struct ColFace*		m_pIndex;
	VECTOR3				m_vNormalH;
	float				m_fHeight;
	VECTOR3				m_vVertexH[3];

	// ���b�V���ڐG����z��
	// (vector�N���X�͒x�����ߔz����g�p����)
	ColMesh				m_ColArray[MCKTBL_MAX];
	int					m_nNum;			// �z��̗v�f��

	// ���������x�}�b�v
	ChkColMesh			m_ChkColMesh[MCKTBL_MAX];

	// �R���W�����ړ��p
	bool				m_bMoveFlag;	// �ړ����邩�ǂ��� �ړ��̎� �^
	MATRIX4X4			m_mWorldOld;	// �ړ��}�g���b�N�X�i���݈ʒu�̈�O�j
	MATRIX4X4			m_mWorld;		// �ړ��}�g���b�N�X�̌��݈ʒu
	MATRIX4X4			m_mWorldInv;	// �ړ��}�g���b�N�X�̌��݈ʒu�̋t�}�g���b�N�X

public:
	CCollision();		   // -- 2024.3.13
	CCollision(CFbxMeshCtrl* pFbxMeshCtrl);   // -- 2021.2.4

	~CCollision();

	/// <summary>
	/// FbxMesh�t�@�C������A�R���W��������f�[�^���쐬����
	/// ����f�[�^�͍��W���_�ɐݒ肳���
	/// </summary>
	/// <param name="">���f���f�[�^�̃p�X</param>
	/// <returns>true:����  false:�t�@�C����������Ȃ�</returns>
	bool	AddFbxLoad( const TCHAR*);

	/// <summary>
	/// FbxMesh�t�@�C������A�R���W��������f�[�^���쐬����
	/// ����f�[�^��ݒ肷����W���w�肷��
	/// </summary>
	/// <param name="">���f���f�[�^�̃p�X</param>
	/// <param name="vOffset">�ݒ肷����W</param>
	/// <returns>true:����  false:�t�@�C����������Ȃ�</returns>
	bool	AddFbxLoad( const TCHAR*, const VECTOR3& vOffset);

	/// <summary>
	/// FbxMesh�t�@�C������A�R���W��������f�[�^���쐬����
	/// ����f�[�^��ݒ肷�郏�[���h�}�g���b�N�X���w�肷��
	/// </summary>
	/// <param name="">���f���f�[�^�̃p�X</param>
	/// <param name="mOffset">�ݒ肷��}�g���b�N�X</param>
	/// <returns>true:����  false:�t�@�C����������Ȃ�</returns>
	bool	AddFbxLoad( const TCHAR*, const MATRIX4X4& mOffset);

	/// <summary>
	/// FbxMesh�f�[�^����A�R���W��������f�[�^���쐬����
	/// ����f�[�^�͍��W���_�ɐݒ肳���
	/// </summary>
	/// <param name="">���b�V���f�[�^�[</param>
	/// <returns>true:����  false:�t�@�C����������Ȃ�</returns>
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh);

	/// <summary>
	/// FbxMesh�f�[�^����A�R���W��������f�[�^���쐬����
	/// ����f�[�^��ݒ肷����W���w�肷��
	/// </summary>
	/// <param name="">���b�V���f�[�^�[</param>
	/// <param name="vOffset">�ݒ肷����W</param>
	/// <returns>true:����  false:�t�@�C����������Ȃ�</returns>
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh, const VECTOR3& vOffset);

 	/// <summary>
	/// FbxMesh�f�[�^����A�R���W��������f�[�^���쐬����
	/// ����f�[�^��ݒ肷�郏�[���h�}�g���b�N�X���w�肷��
	/// </summary>
	/// <param name="">���b�V���f�[�^�[</param>
	/// <param name="vOffset">�ݒ肷�郏�[���h�}�g���b�N�X</param>
	/// <returns>true:����  false:�t�@�C����������Ȃ�</returns>
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh, const MATRIX4X4& mOffset);

	/// <summary>
	/// �����Ƃ̓����蔻����s��
	/// �n�_�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// </summary>
	/// <param name="startIn">�����̎n�_</param>
	/// <param name="endIn">�����̏I�_</param>
	/// <param name="vHit">���������ʒu������ꏊ(Out)</param>
	/// <param name="vNormal">���������ʒu�̖@��������ꏊ(Out)</param>
	/// <returns>�������Ă����true</returns>
	bool    IsCollisionLay(const VECTOR3& startIn, const VECTOR3& endIn, VECTOR3& vHit, VECTOR3& vNormal);

	/// <summary>
	/// ���̂Ƃ̓����蔻����s��
	/// �ړ��J�n�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// </summary>
	/// <param name="startIn">�ړ��J�n�_</param>
	/// <param name="endIn">�ړ��I���_</param>
	/// <param name="fRadius">���a</param>
	/// <param name="vHit">���������ʒu������ꏊ(Out)</param>
	/// <param name="vNormal">���������ʒu�̖@��������ꏊ(Out)</param>
	/// <returns>�������Ă����true</returns>
	bool    IsCollisionSphere(const VECTOR3& startIn, const VECTOR3& endIn, const float& fRadius, VECTOR3& vHit, VECTOR3& vNormal);

	/// <summary>
	/// ���̂̈ړ������Ƃ̓����蔻����s��
	/// �ړ��J�n�_�����ԋ߂��|���S���œ����蔻����s���܂�
	/// �������Ă����ꍇ�A���̂��|���S���̊O�ɏo��悤�Ɉړ��I���_position�̒l���ύX����܂�
	/// </summary>
	/// <param name="positionOld">�ړ��O�_</param>
	/// <param name="position">�ړ���_(In/Out)</param>
	/// <param name="fRadius">���a</param>
	/// <returns>�������Ă����true</returns>
	bool    IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

	/// <summary>
	/// ���̂̈ړ������Ƃ̓����蔻����s��
	/// �ړ��J�n�_�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// �������Ă����ꍇ�A���̂��|���S���̊O�ɏo��悤�Ɉړ��I���_position�̒l���ύX����܂�
	/// </summary>
	/// <param name="positionOld">�ړ��O�_</param>
	/// <param name="position">�ړ���_(In/Out)</param>
	/// <param name="vHit">���������ʒu������ꏊ(Out)</param>
	/// <param name="vNormal">���������ʒu�̖@��������ꏊ(Out)</param>
	/// <param name="fRadius">���a</param>
	/// <returns>�������Ă����true</returns>
	bool    IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);

	/// <summary>
	/// ���̂̈ړ������Ƃ̏d�͂��������������蔻����s��
	/// �ړ��J�n�_�����ԋ߂��|���S���œ����蔻����s���܂�
	/// �������Ă����ꍇ�A���̂��|���S���̊O�ɏo��悤�Ɉړ��I���_position�̒l���ύX����܂�
	/// </summary>
	/// <param name="positionOld">�ړ��O�_</param>
	/// <param name="position">�ړ���_(In/Out)</param>
	/// <param name="fRadius">���a</param>
	/// <returns>�R���W��������̌���</returns>
	CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, float fRadius = 0.2f);

	/// ���̂̈ړ������Ƃ̏d�͂��������������蔻����s��
	/// �ړ��J�n�_�����ԋ߂��|���S���œ����蔻����s���܂�
	/// �������Ă����ꍇ�A���̂��|���S���̊O�ɏo��悤�Ɉړ��I���_position�̒l���ύX����܂�
	/// </summary>
	/// <param name="positionOld">�ړ��O�_</param>
	/// <param name="position">�ړ���_(In/Out)</param>
	/// <param name="vHit">���������ʒu������ꏊ(Out)</param>
	/// <param name="vNormal">���������ʒu�̖@��������ꏊ(Out)</param>
	/// <param name="fRadius">���a</param>
	/// <returns>�R���W��������̌���</returns>
	CollRet IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& pisition, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);

	/// <summary>
	/// �ړ��}�b�v�̃}�g���b�N�X�̏������Ɛݒ�A�Q�Ɠ�
	/// (�ʏ�̃}�b�v�ɂ͊֌W���Ȃ�)
	/// </summary>
	/// <param name="">���[���h�}�g���b�N�X</param>
	void	InitWorldMatrix(const MATRIX4X4&);
	void	SetWorldMatrix(const MATRIX4X4&);
	void	SaveWorldMatrix() { m_mWorldOld = m_mWorld; }
	MATRIX4X4 WorldMatrix(){ return m_mWorld;}

	/// <summary>
	/// �}�b�v�t���O�̎Q��
	/// </summary>
	/// <returns>�ړ��}�b�v�̂Ƃ�true</returns>
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
