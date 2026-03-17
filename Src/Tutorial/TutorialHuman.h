#pragma once
#include "TutorialAnimal.h"

// 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ莠ｺ髢薙け繝ｩ繧ｹ //
class CTutorialHuman : public Object3D
{
public:
    // 謖・ｮ壻ｽ咲ｽｮ縺ｫ繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ莠ｺ髢薙ｒ逕滓・縺吶ｋ
    // @param pos 逕滓・菴咲ｽｮ //
    CTutorialHuman(const VECTOR3& pos);
    ~CTutorialHuman();

private:
    void Update() override;

    bool m_inSight;  // 繝励Ξ繧､繝､繝ｼ縺瑚ｦ也阜蜀・↓縺・ｋ縺九←縺・° //
};
