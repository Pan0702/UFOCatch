#pragma once

#include <vector>
#include <list>
#include <string>
#include "MyMath.h"
#include "Animator.h"

/// <summary>Transform、メッシュ、アニメーション、コライダーを持つ3Dオブジェクト基底クラス</summary>
class Object3D;
/// <summary>汎用ユーティリティで使う Fbx Mesh の情報と処理をまとめる型</summary>
class CFbxMesh;

/// <summary>
/// モデルデータとの当たり判定処理を行います
/// また、当たり、押し、戻し処理を行います
/// </summary>
class MeshCollider
{
public:
    /// <summary>汎用ユーティリティで使う Vertex の情報と処理をまとめる型</summary>
    struct Vertex
    {
        VECTOR3 pos;
        UINT bone[4];
        VECTOR4 weits; // (weightsのタイポと思われますが元の定義を維持)
    };

    /// <summary>汎用ユーティリティで使う Coll Info の情報と処理をまとめる型</summary>
    struct CollInfo
    {
        VECTOR3 hitPosition; // 衝突した位置
        VECTOR3 normal; // 法線
        Vertex triangle[3]; // 頂点
        int meshNo; // メッシュ番号
    };

    /// MeshCollider を初期化する
    MeshCollider();
    /// MeshCollider を初期化する
    /// @param object 対象オブジェクト
    MeshCollider(Object3D* object);
    /// MeshCollider の終了処理を行う
    ~MeshCollider();

    /// From Mesh を作成する
    /// @param meshIn meshIn に渡す値
    /// @param animatorIn animatorIn に渡す値
    void MakeFromMesh(CFbxMesh* meshIn, Animator* animatorIn = nullptr);

    /// From File を作成する
    /// @param fileName ファイル名
    void MakeFromFile(std::string fileName);

    /// Bone No を選択する
    /// @return 処理結果の数値
    int SelectBoneNo(Vertex vt[3]);

    /// transform Skin Vertices の処理を行う
    void transformSkinVertices();

    /// Collision Line を確認する
    /// @param trans trans に渡す値
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @param hitOut 衝突情報の出力先
    /// @return 成功または条件を満たす場合 true
    bool CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, CollInfo* hitOut = nullptr);

    /// Collision Sphere を確認する
    /// @param trans trans に渡す値
    /// @param center center に渡す値
    /// @param radius 半径
    /// @param hitOut 衝突情報の出力先
    /// @return 成功または条件を満たす場合 true
    bool CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut = nullptr);

    /// Collision Sphere List を確認する
    /// @param trans trans に渡す値
    /// @param center center に渡す値
    /// @param radius 半径
    /// @return 取得した要素一覧
    std::list<MeshCollider::CollInfo> CheckCollisionSphereList(const MATRIX4X4& trans, const VECTOR3& center,
                                                               float radius);

    /// カプセルとの交差判定を行う
    /// ポリゴンの表面のみ判定し、裏面は判定しません
    /// カプセルのp1の点から一番近いポリゴンの交差情報を返します
    /// infoがnullptrの場合、交差判定のみ行い、交差したか否かのみ返します
    /// 複数のポリゴンと接触する可能性があるため、CheckCollisionCapsuleList関数の使用を推奨します
    /// @param trans このコライダーの位置行列
    /// @param p1 カプセルの点1
    /// @param p2 カプセルの点2
    /// @param radius 球の半径
    /// @param hitOut 衝突したポリゴンの情報格納先
    /// @return 衝突していたらtrue
    //bool CheckCollisionCapsule(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius, CollInfo* hitOut = nullptr);

    /// カプセルとの交差判定を行う
    /// 交差している全ポリゴンの情報を返します
    /// ポリゴンの表面のみ判定し、裏面は判定しません
    /// @param trans このコライダーの位置行列
    /// @param p1 カプセルの点1
    /// @param p2 カプセルの点2
    /// @param radius 球の半径
    /// @return 衝突した全ポリゴンの情報リスト
    //std::list<MeshCollider::CollInfo> CheckCollisionCapsuleList(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius);

    //bool CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* vertexes, CollInfo* info = nullptr);

    /// Bounding Line を確認する
    /// @param trans trans に渡す値
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CheckBoundingLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to);

    /// Ball を取得する
    /// @param center center に渡す値
    /// @param radius 半径
    void GetBall(VECTOR3* center, float* radius) const
    {
        *center = bBall.center;
        *radius = bBall.radius;
    }

private:
    Object3D* parent;

    /// <summary>汎用ユーティリティで使う Bounding Box の情報と処理をまとめる型</summary>
    struct BoundingBox
    {
        VECTOR3 min;
        VECTOR3 max;

        BoundingBox()
        {
            min = VECTOR3(0, 0, 0), max = VECTOR3(0, 0, 0);
        }
    };

    /// <summary>汎用ユーティリティで使う Bounding Ball の情報と処理をまとめる型</summary>
    struct BoundingBall
    {
        VECTOR3 center;
        float radius;

        BoundingBall()
        {
            center = VECTOR3(0, 0, 0);
            radius = 0.0f;
        }
    };

    /// <summary>汎用ユーティリティで使う Polygon Info の情報と処理をまとめる型</summary>
    struct PolygonInfo
    {
        int indices[3]; // 頂点番号
        VECTOR3 normal; // 面の法線
    };

    CFbxMesh* mesh;
    Animator* animator;
    int id;
    int frame;

    std::vector<std::vector<PolygonInfo>> polygons;
    std::vector<std::vector<Vertex>> vertices;

public:
    BoundingBox bBox;
    BoundingBall bBall;

private:
    /// check Polygon To Line を返す
    /// @param m m に渡す値
    /// @param info info に渡す値
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @param hit hit に渡す値
    /// @return 成功または条件を満たす場合 true
    bool checkPolygonToLine(const int m, const PolygonInfo& info, const VECTOR3& from, const VECTOR3& to,
                            CollInfo* hit = nullptr);
    /// check Polygon To Sphere を返す
    /// @param m m に渡す値
    /// @param info info に渡す値
    /// @param center center に渡す値
    /// @param radius 半径
    /// @param hit hit に渡す値
    /// @return 成功または条件を満たす場合 true
    bool checkPolygonToSphere(const int m, const PolygonInfo& info, const VECTOR3& center, float radius,
                              CollInfo* hit = nullptr);
};
