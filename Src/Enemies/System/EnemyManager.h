#pragma once
#include "../../Common/Object3D.h"
#include "ModelRegistry.h"
#include "EnemyQuadTree.h"
#include "../../Stage/StageQuadTree.h"
#include <vector>

class CPlayer;
class CEnemyBase;
class CStageObject;
class CFbxMesh;

// 蜍慕黄邂｡逅・け繝ｩ繧ｹ・医ヵ繧｡繧ｵ繝ｼ繝会ｼ・
// ModelRegistry縺ｨQuadTreeIndex繧堤ｵｱ蜷医＠縺ｦ菴ｿ逕ｨ
class CEnemyManager : public Object3D
{
public:
    CEnemyManager();
    ~CEnemyManager();

    void SetRotationY(const float& angle);

    // 繝｡繝・す繝･蜿門ｾ暦ｼ・odelRegistry縺ｫ蟋碑ｭｲ・・
    CFbxMesh* MeshList(const std::string& str) const;

    // 謨ｵ繝ｪ繧ｹ繝育ｮ｡逅・ｼ・EnemyBase縺ｮ逕滓・/遐ｴ譽・凾縺ｫ閾ｪ蜍募他縺ｳ蜃ｺ縺暦ｼ・
    void RegisterEnemy(CEnemyBase* enemy);
    void UnregisterEnemy(CEnemyBase* enemy);
    const std::vector<CEnemyBase*>& GetAllEnemies() const { return m_enemies; }

    // 蜻ｨ霎ｺ繧ｨ繝阪Α繝ｼ蜿門ｾ暦ｼ・uadTreeIndex縺ｫ蟋碑ｭｲ・・
    std::vector<CEnemyBase*> GetNearbyEnemies(
        CEnemyBase* pObj,const VECTOR2& pos,const VECTOR2& size) const;

    // 邨ｱ險域ュ蝣ｱ・・uadTreeIndex縺ｫ蟋碑ｭｲ・・
    const CollisionStats& GetCollisionStats() const;
    void ResetCollisionStats() const;

    // 髱咏噪繝・Μ繝ｼ繧偵す繝ｼ繝ｳ蛻晄悄蛹門ｾ後↓1蝗樊ｧ狗ｯ峨☆繧・
    void BuildStaticTree() const;

    // 霑代￥縺ｮ髱咏噪繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜿門ｾ暦ｼ・taticQuadTreeIndex縺ｫ蟋碑ｭｲ・・
    std::vector<CStageObject*> GetNearbyStageObjects(
        const VECTOR2& pos, const VECTOR2& size) const;

    // 蜷・け繝ｩ繧ｹ縺ｸ縺ｮ逶ｴ謗･繧｢繧ｯ繧ｻ繧ｹ・亥ｿ・ｦ√↓蠢懊§縺ｦ・・
    CModelRegistry* GetModelRegistry() const { return m_pModelRegistry; }
    CEnemyQuadTree* GetQuadTreeIndex() const { return m_pQuadTreeIndex; }
    CStageQuadTree* GetStaticQuadTreeIndex() const { return m_pStaticQuadTreeIndex; }

public:
    CPlayer* m_pPlayer;

private:
    void Update() override;

    CModelRegistry* m_pModelRegistry;
    CEnemyQuadTree* m_pQuadTreeIndex;
    CStageQuadTree* m_pStaticQuadTreeIndex;
    std::vector<CEnemyBase*> m_enemies;

    // 莠呈鋤諤ｧ縺ｮ縺溘ａ谿九☆・域悴菴ｿ逕ｨ・・
    CFbxMesh* m_pMesh;
    class CMeshCollider* m_pMeshCol;
};

