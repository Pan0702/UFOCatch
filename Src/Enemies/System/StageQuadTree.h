#pragma once
#include <vector>
#include "../Liner4Tree.h"

class CStageObject;

// 髱咏噪繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝育畑縺ｮ遨ｺ髢薙う繝ｳ繝・ャ繧ｯ繧ｹ
// 繧ｷ繝ｼ繝ｳ蛻晄悄蛹匁凾縺ｫ1蝗槭□縺腺uild()繧貞他縺ｶ・域ｯ弱ヵ繝ｬ繝ｼ繝譖ｴ譁ｰ荳崎ｦ・ｼ・
class CStageQuadTree
{
public:
    CStageQuadTree();
    ~CStageQuadTree();

    // 繧ｷ繝ｼ繝ｳ縺ｮ繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝育函謌仙ｾ後↓1蝗槫他縺ｶ
    void Build() const;

    // 霑代￥縺ｮ髱咏噪繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜿門ｾ・
    std::vector<CStageObject*> GetNearbyObjects(
        const VECTOR2& pos,
        const VECTOR2& size) const;

private:
    CLiner4Tree<CStageObject>* m_pTree;
};