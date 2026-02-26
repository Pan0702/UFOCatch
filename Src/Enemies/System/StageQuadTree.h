#pragma once
#include <vector>
#include "../Liner4Tree.h"

class CStageObject;

// 静的ステージオブジェクト用の空間インデックス
// シーン初期化時に1回だけBuild()を呼ぶ（毎フレーム更新不要）
class CStageQuadTree
{
public:
    CStageQuadTree();
    ~CStageQuadTree();

    // シーンのステージオブジェクト生成後に1回呼ぶ
    void Build() const;

    // 近くの静的オブジェクトを取得
    std::vector<CStageObject*> GetNearbyObjects(
        const VECTOR2& pos,
        const VECTOR2& size) const;

private:
    CLiner4Tree<CStageObject>* m_pTree;
};