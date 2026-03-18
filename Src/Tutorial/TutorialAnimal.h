#pragma once
#include "../Common/Object3D.h"

class CGround;

// 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ蜍慕黄繧ｯ繝ｩ繧ｹ //
class CTutorialAnimal : public Object3D
{
public:
    // 謖・ｮ壻ｽ咲ｽｮ縺ｫ繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ蜍慕黄繧堤函謌舌☆繧・
    // @param pos 逕滓・菴咲ｽｮ //
    CTutorialAnimal(const VECTOR3& pos);
    ~CTutorialAnimal();
    
    //縲Score繧定ｶｳ縺励※繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒDestroy//
    void Destroy();

private:


    void Update() override;

    // 驥榊鴨繧帝←逕ｨ縺吶ｋ //
    void ApplyGravity();

    float m_velocityY = 0.0f;      // Y霆ｸ譁ｹ蜷代・騾溷ｺｦ //
    CGround* m_pGround = nullptr;  // 蝨ｰ髱｢繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //
};

