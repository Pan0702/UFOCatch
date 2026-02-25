#pragma once

#include <vector>
#include <list>
#include <string>
#include "MyMath.h"
#include "Animator.h"

class Object3D;
class CFbxMesh;

/// <summary>
/// モデルデータとの当たり判定処理を行います
/// 保持しているポリゴンの頂点座標・法線・重み付き頂点を返します
/// また、当たり、押し、戻し処理を行います
/// </summary>
class MeshCollider
{
public:
    struct Vertex {
       VECTOR3 pos;
       UINT    bone[4];
       VECTOR4 weits; // (weightsのタイポと思われますが元の定義を維持)
    };

    struct CollInfo {
       VECTOR3 hitPosition; // 衝突した位置
       VECTOR3 normal;      // 法線
       Vertex  triangle[3]; // 頂点
       int     meshNo;      // メッシュ番号
    };

    MeshCollider();
    MeshCollider(Object3D* object);
    ~MeshCollider();

    /// <summary>
    /// FbxMeshデータから、当たりデータを作成する
    /// スキンメッシュの場合は必ずAnimatorを指定してください
    /// スタティックメッシュの場合は指定不要です
    /// </summary>
    /// <param name="meshIn">メッシュデータ</param>
    /// <param name="animatorIn">アニメーター</param>
    void MakeFromMesh(CFbxMesh* meshIn, Animator* animatorIn = nullptr);

    /// <summary>
    /// meshファイルから、当たりデータを作成する
    /// 当たり判定用に軽量化したモデルを使う場合はこちらを使ってください
    /// なお、スキンメッシュには適用できません
    /// </summary>
    /// <param name="fileName">モデルデータのパス</param>
    void MakeFromFile(std::string fileName);

    /// <summary>
    /// ポリゴンの中心頂点からウェイト値を参照しボーン番号を一つ返す
    /// </summary>
    /// <param name="vt">頂点[3]</param>
    /// <returns>ボーン番号</returns>
    int SelectBoneNo(Vertex vt[3]);

    /// <summary>
    /// スキンメッシュの頂点をボーン配列で変形させる
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
    /// <param name="hitOut">交差したポリゴンの情報を格納する場所</param>
    /// <returns>交差していたらtrue</returns>
    bool CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, CollInfo* hitOut = nullptr);

    /// <summary>
    /// 球との交差判定を行う
    /// 
    /// 球の中心座標から一番近いポリゴンの交差情報を返します
    /// infoがnullptrの場合、交差判定のみ行い、交差したか否かのみ返します
    /// 
    /// 複数のポリゴンと接触する可能性があるため、CheckCollisionSphereList関数の使用を推奨します
    /// </summary>
    /// <param name="trans">このコライダーの位置行列</param>
    /// <param name="center">球の中心座標</param>
    /// <param name="radius">球の半径</param>
    /// <param name="hitOut">衝突したポリゴンの情報格納先</param>
    /// <returns>衝突していたらtrue</returns>
    bool CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut = nullptr);

    /// <summary>
    /// 球との交差判定を行う
    /// 
    /// 交差しているポリゴンすべての情報を返します
    /// 交差しているものがなければ、returnのsizeは0になります
    /// </summary>
    /// <param name="trans">このコライダーの位置行列</param>
    /// <param name="center">球の中心座標</param>
    /// <param name="radius">球の半径</param>
    /// <returns>衝突した全ポリゴンの情報リスト</returns>
    std::list<MeshCollider::CollInfo> CheckCollisionSphereList(const MATRIX4X4& trans, const VECTOR3& center, float radius);

    /// <summary>
    /// カプセルとの交差判定を行う
    /// ポリゴンの表面のみ判定し、裏面は判定しません
    /// 
    /// カプセルのp1の点から一番近いポリゴンの交差情報を返します
    /// infoがnullptrの場合、交差判定のみ行い、交差したか否かのみ返します
    /// 
    /// 複数のポリゴンと接触する可能性があるため、CheckCollisionCapsuleList関数の使用を推奨します
    /// </summary>
    /// <param name="trans">このコライダーの位置行列</param>
    /// <param name="p1">カプセルの点1</param>
    /// <param name="p2">カプセルの点2</param>
    /// <param name="radius">球の半径</param>
    /// <param name="hitOut">衝突したポリゴンの情報格納先</param>
    /// <returns>衝突していたらtrue</returns>
    //bool CheckCollisionCapsule(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius, CollInfo* hitOut = nullptr);

    /// <summary>
    /// カプセルとの交差判定を行う
    /// 交差している全ポリゴンの情報を返します
    /// 
    /// ポリゴンの表面のみ判定し、裏面は判定しません
    /// </summary>
    /// <param name="trans">このコライダーの位置行列</param>
    /// <param name="p1">カプセルの点1</param>
    /// <param name="p2">カプセルの点2</param>
    /// <param name="radius">球の半径</param>
    /// <returns>衝突した全ポリゴンの情報リスト</returns>
    //std::list<MeshCollider::CollInfo> CheckCollisionCapsuleList(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius);

    //bool CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* vertexes, CollInfo* info = nullptr);

    /// <summary>
    /// 境界線との交差判定を行う
    /// 始点から一番近いポリゴンの交差情報を返します
    /// ポリゴンの表面のみ判定し、裏面は判定しません
    /// </summary>
    /// <param name="trans">このコライダーの位置行列</param>
    /// <param name="from">線分の始点</param>
    /// <param name="to">線分の終点</param>
    /// <returns>交差していたらtrue</returns>
    bool CheckBoundingLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to);

    /// <summary>
    /// 境界球の情報を取得
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
public:
    BoundingBox bBox;
    BoundingBall bBall;
private:
    bool checkPolygonToLine(const int m, const PolygonInfo& info, const VECTOR3& from, const VECTOR3& to, CollInfo* hit = nullptr);
    bool checkPolygonToSphere(const int m, const PolygonInfo& info, const VECTOR3& center, float radius, CollInfo* hit = nullptr);
};