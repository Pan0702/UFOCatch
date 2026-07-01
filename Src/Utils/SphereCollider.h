#pragma once
#include "FbxMesh.h"
#include "../05_CommonFile/Object3D.h"

/// <summary>汎用ユーティリティで使う Sphere Collider の情報と処理をまとめる型</summary>
class SphereCollider
{
public:
    /// SphereCollider を初期化する
    SphereCollider();
    /// SphereCollider を初期化する
    /// @param object 対象オブジェクト
    SphereCollider(Object3D* object);
    /// From Mesh を作成する
    /// @param mesh mesh に渡す値
    void MakeFromMesh(CFbxMesh* mesh);
    /// From File を作成する
    /// @param fileName ファイル名
    void MakeFromFile(std::string fileName);
    /// Center を返す
    /// @return 3次元ベクトル
    VECTOR3 Center() { return center; }
    /// Radius を返す
    /// @return 計算結果の値
    float Radius() { return radius; }

private:
    VECTOR3 center;
    float radius;
};
