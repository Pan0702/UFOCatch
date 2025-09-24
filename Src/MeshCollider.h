#pragma once

#include <vector>
#include <list>
#include <string>
#include "MyMath.h"
#include "Animator.h"

class Object3D;
class CFbxMesh;

/// <summary>
/// モデルデータとの当たり判定を行います
/// 当たっているポリゴンの頂点座標、法線、当たった点を返します
/// 線分、または、球で、判定を行います
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
		VECTOR3 hitPosition; // 当たった位置
		VECTOR3 normal;		 // 法線
		Vertex  triangle[3]; // 頂点
		int     meshNo;      // メッシュ番号
	};

	MeshCollider();
	MeshCollider(Object3D* object);
	~MeshCollider();

	/// <summary>
	/// FbxMeshデータから、判定データを作成する
	/// スキンメッシュの場合には必ずAnimatorを指定してください
	/// スタティックメッシュの場合は指定不要です
	/// </summary>
	/// <param name="mesh">メッシュデータ</param>
	/// <param name="animator">アニメーター</param>
	void MakeFromMesh(CFbxMesh* meshIn, Animator* animatorIn = nullptr);

	/// <summary>
	/// meshファイルから、判定データを作成する
	/// 当たり判定用に軽量化モデルを使う場合はこちらを使ってください
	/// なお、スキンメッシュには適用できません
	/// </summary>
	/// <param name="fileName">モデルデータのパス</param>
	void MakeFromFile(std::string fileName);

	/// <summary>
	/// ポリゴンの中で頂点が影響を与えるボーン番号を選択する
	/// </summary>
	/// <param name="vt">頂点[3]</param>
	/// <returns>ボーン番号</returns>
	int SelectBoneNo(Vertex  vt[3]);

	/// <summary>
	/// スキンメッシュの頂点をボーン配列で変形する
	/// </summary>
	void transformSkinVertices();

	/// <summary>
	/// 線分との当たり判定を行う
	/// 始点から一番近いポリゴンの当たり判定情報を返します
	/// ポリゴンの表面のみ判定し、裏面は判定しません
	/// </summary>
	/// <param name="trans">このコライダーの位置情報</param>
	/// <param name="from">線分の始点</param>
	/// <param name="to">線分の終点</param>
	/// <param name="info">当たったポリゴンの情報を入れる場所</param>
	/// <returns>当たっていればtrue</returns>
	bool CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, CollInfo* hitOut = nullptr);

	/// <summary>
	/// 球体との当たり判定を行う
	/// 
	/// 球の中心から一番近いポリゴンの当たり判定情報を返します
	/// infoがnullptrの場合は、当たり情報を返さず、当たったかのみ返します
	/// 
	/// 複数のポリゴンが当たる可能性があるので、CheckCollisionSphereList関数を使用することを推奨します
	/// </summary>
	/// <param name="trans">このコライダーの位置情報</param>
	/// <param name="center">球の中心座標</param>
	/// <param name="radius">球の半径</param>
	/// <param name="info">当たったポリゴンの情報を入れる場所</param>
	/// <returns>当たっていればtrue</returns>
	bool CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut = nullptr);

	/// <summary>
	/// 球体との当たり判定を行う
	/// 
	/// 当たっているポリゴンすべての情報を返します
	/// 当たっているものがなければ、returnのsizeが0になります
	/// </summary>
	/// <param name="trans">このコライダーの位置情報</param>
	/// <param name="center">球の中心座標</param>
	/// <param name="radius">球の半径</param>
	/// <returns>当たったポリゴン全ての位置情報</returns>
	std::list<MeshCollider::CollInfo> CheckCollisionSphereList(const MATRIX4X4& trans, const VECTOR3& center, float radius);

	/// <summary>
	/// カプセルとの当たり判定を行う
	/// ポリゴンの表面のみ判定し、裏面は判定しません
	/// 
	/// カプセルのp1の点から一番近いポリゴンの当たり判定情報を返します
	/// infoがnullptrの場合は、当たり情報を返さず、当たったかのみ返します
	/// 
	/// 複数のポリゴンが当たる可能性があるので、CheckCollisionCapsuleList関数を使用することを推奨します
	/// </summary>
	/// <param name="trans">このコライダーの位置情報</param>
	/// <param name="p1">カプセルの点１</param>
	/// <param name="p2">カプセルの点２</param>
	/// <param name="radius">球の半径</param>
	/// <param name="info">当たったポリゴンの情報を入れる場所</param>
	/// <returns>当たっていればtrue</returns>
	//bool CheckCollisionCapsule(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius, CollInfo* hitOut = nullptr);

	/// <summary>
	/// カプセルとの当たり判定を行う
	/// 当たっているポリゴンすべての情報を返します
	/// 
	/// ポリゴンの表面のみ判定し、裏面は判定しません
	/// </summary>
	/// <param name="trans">このコライダーの位置情報</param>
	/// <param name="p1">カプセルの点１</param>
	/// <param name="p2">カプセルの点２</param>
	/// <param name="radius">球の半径</param>
	/// <returns>当たったポリゴン全ての位置情報</returns>
	//std::list<MeshCollider::CollInfo> CheckCollisionCapsuleList(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius);

	//bool CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* vertexes, CollInfo* info = nullptr);

	/// <summary>
	/// 線分との当たり判定を行う
	/// 始点から一番近いポリゴンの当たり判定情報を返します
	/// ポリゴンの表面のみ判定し、裏面は判定しません
	/// </summary>
	/// <param name="trans">このコライダーの位置情報</param>
	/// <param name="from">線分の始点</param>
	/// <param name="to">線分の終点</param>
	/// <param name="info">当たったポリゴンの情報を入れる場所</param>
	/// <returns>当たっていればtrue</returns>
	bool CheckBoundingLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to);

	/// <summary>
	/// Ballの情報を返す
	/// </summary>
	/// <param name="center">中心位置(Out)</param>
	/// <param name="radius">半径(Out)</param>
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
		int indices[3]; // 頂点番号
		VECTOR3 normal; // 面の法線
	};
	CFbxMesh* mesh;
	Animator* animator;
	int       id;
	int       frame;

	std::vector<std::vector<PolygonInfo>> polygons;
	std::vector<std::vector<Vertex>> vertices;
	BoundingBox bBox;
	BoundingBall bBall;

	bool checkPolygonToLine(const int m, const PolygonInfo& info, const VECTOR3& from, const VECTOR3& to, CollInfo* hit = nullptr);
	bool checkPolygonToSphere(const int m, const PolygonInfo& info, const VECTOR3& center, float radius, CollInfo* hit = nullptr);
};