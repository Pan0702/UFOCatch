#pragma once
#include "../Utils/MeshCollider.h"
/// <summary>ステージで使う Stage Object の情報と処理をまとめる型</summary>
class CStageObject;

//床に当たったかの結果を保持
struct GroundHitResult
{
    bool hit = false; //当たったか//
    float y = 0.0f;
    MeshCollider::CollInfo collInfo; //当たった情報
    CStageObject* object = nullptr; //当たったオブジェクト
};
