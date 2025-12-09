#pragma once

#include <vector>
#include <list>
#include <string>
#include "MyMath.h"
#include "Animator.h"

class Object3D;
class CFbxMesh;

/// <summary>
/// ���f���f�[�^�Ƃ̓����蔻����s���܂�
/// �������Ă���|���S���̒��_���W�A�@���A���������_��Ԃ��܂�
/// �����A�܂��́A���ŁA������s���܂�
/// </summary>
class MeshCollider
{
public:
	struct Vertex {
		VECTOR3 pos;
		UINT    bone[4];
		VECTOR4 weits;
	};

	struct CollInfo {
		VECTOR3 hitPosition; // ���������ʒu
		VECTOR3 normal;		 // �@��
		Vertex  triangle[3]; // ���_
		int     meshNo;      // ���b�V���ԍ�
	};

	MeshCollider();
	MeshCollider(Object3D* object);
	~MeshCollider();

	/// <summary>
	/// FbxMesh�f�[�^����A����f�[�^���쐬����
	/// �X�L�����b�V���̏ꍇ�ɂ͕K��Animator���w�肵�Ă�������
	/// �X�^�e�B�b�N���b�V���̏ꍇ�͎w��s�v�ł�
	/// </summary>
	/// <param name="mesh">���b�V���f�[�^</param>
	/// <param name="animator">�A�j���[�^�[</param>
	void MakeFromMesh(CFbxMesh* meshIn, Animator* animatorIn = nullptr);

	/// <summary>
	/// mesh�t�@�C������A����f�[�^���쐬����
	/// �����蔻��p�Ɍy�ʉ����f�����g���ꍇ�͂�������g���Ă�������
	/// �Ȃ��A�X�L�����b�V���ɂ͓K�p�ł��܂���
	/// </summary>
	/// <param name="fileName">���f���f�[�^�̃p�X</param>
	void MakeFromFile(std::string fileName);

	/// <summary>
	/// �|���S���̒��Œ��_���e����^����{�[���ԍ���I������
	/// </summary>
	/// <param name="vt">���_[3]</param>
	/// <returns>�{�[���ԍ�</returns>
	int SelectBoneNo(Vertex  vt[3]);

	/// <summary>
	/// �X�L�����b�V���̒��_���{�[���z��ŕό`����
	/// </summary>
	void transformSkinVertices();

	/// <summary>
	/// 線分との交差判定を行います。
	/// 始点から一番近いポリゴンの交差点を返します。
	/// ポリゴンの表面のみ判定し、裏面は判定しません。
	/// </summary>
	/// <param name="trans">このコライダーの位置情報</param>
	/// <param name="from">線分の始点</param>
	/// <param name="to">線分の終点</param>
	/// <param name="info">交差したポリゴンの情報を格納する場所</param>
	/// <returns>交差していたらtrue</returns>
	bool CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, CollInfo* hitOut = nullptr);

	/// <summary>
	/// ���̂Ƃ̓����蔻����s��
	/// 
	/// ���̒��S�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// info��nullptr�̏ꍇ�́A���������Ԃ����A�����������̂ݕԂ��܂�
	/// 
	/// �����̃|���S����������\��������̂ŁACheckCollisionSphereList�֐����g�p���邱�Ƃ𐄏����܂�
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="center">���̒��S���W</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="info">���������|���S���̏�������ꏊ</param>
	/// <returns>�������Ă����true</returns>
	bool CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut = nullptr);

	/// <summary>
	/// ���̂Ƃ̓����蔻����s��
	/// 
	/// �������Ă���|���S�����ׂĂ̏���Ԃ��܂�
	/// �������Ă�����̂��Ȃ���΁Areturn��size��0�ɂȂ�܂�
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="center">���̒��S���W</param>
	/// <param name="radius">���̔��a</param>
	/// <returns>���������|���S���S�Ă̈ʒu���</returns>
	std::list<MeshCollider::CollInfo> CheckCollisionSphereList(const MATRIX4X4& trans, const VECTOR3& center, float radius);

	/// <summary>
	/// �J�v�Z���Ƃ̓����蔻����s��
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// 
	/// �J�v�Z����p1�̓_�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// info��nullptr�̏ꍇ�́A���������Ԃ����A�����������̂ݕԂ��܂�
	/// 
	/// �����̃|���S����������\��������̂ŁACheckCollisionCapsuleList�֐����g�p���邱�Ƃ𐄏����܂�
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="p1">�J�v�Z���̓_�P</param>
	/// <param name="p2">�J�v�Z���̓_�Q</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="info">���������|���S���̏�������ꏊ</param>
	/// <returns>�������Ă����true</returns>
	//bool CheckCollisionCapsule(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius, CollInfo* hitOut = nullptr);

	/// <summary>
	/// �J�v�Z���Ƃ̓����蔻����s��
	/// �������Ă���|���S�����ׂĂ̏���Ԃ��܂�
	/// 
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="p1">�J�v�Z���̓_�P</param>
	/// <param name="p2">�J�v�Z���̓_�Q</param>
	/// <param name="radius">���̔��a</param>
	/// <returns>���������|���S���S�Ă̈ʒu���</returns>
	//std::list<MeshCollider::CollInfo> CheckCollisionCapsuleList(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius);

	//bool CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* vertexes, CollInfo* info = nullptr);

	/// <summary>
	/// �����Ƃ̓����蔻����s��
	/// �n�_�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="from">�����̎n�_</param>
	/// <param name="to">�����̏I�_</param>
	/// <param name="info">���������|���S���̏�������ꏊ</param>
	/// <returns>�������Ă����true</returns>
	bool CheckBoundingLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to);

	/// <summary>
	/// Ball�̏���Ԃ�
	/// </summary>
	/// <param name="center">���S�ʒu(Out)</param>
	/// <param name="radius">���a(Out)</param>
	void GetBall(VECTOR3* center, float* radius){  *center = bBall.center; *radius = bBall.radius; }

private:
	Object3D* parent;

	struct BoundingBox {
		VECTOR3 min;
		VECTOR3 max;
		BoundingBox() {
			min = VECTOR3(0, 0, 0), max = VECTOR3(0, 0, 0);
		}
	};
	struct BoundingBall {
		VECTOR3 center;
		float radius;
		BoundingBall() {
			center = VECTOR3(0, 0, 0); radius = 0.0f;
		}
	};
	struct PolygonInfo {
		int indices[3]; // ���_�ԍ�
		VECTOR3 normal; // �ʂ̖@��
	};
	CFbxMesh* mesh;
	Animator* animator;
	int       id;
	int       frame;

	std::vector<std::vector<PolygonInfo>> polygons;
	std::vector<std::vector<Vertex>> vertices;
public:
	BoundingBox bBox;
	BoundingBall bBall;
private:
	bool checkPolygonToLine(const int m, const PolygonInfo& info, const VECTOR3& from, const VECTOR3& to, CollInfo* hit = nullptr);
	bool checkPolygonToSphere(const int m, const PolygonInfo& info, const VECTOR3& center, float radius, CollInfo* hit = nullptr);
};