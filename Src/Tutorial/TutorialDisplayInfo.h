#pragma once
#include "../Common/Object3D.h"

// 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｮUI陦ｨ遉ｺ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ //
class CTutorialDisplayInfo : public Object3D
{
public:
    CTutorialDisplayInfo();
    ~CTutorialDisplayInfo();
    // 陦ｨ遉ｺ繧ｿ繧､繝励ｒ險ｭ螳壹☆繧・
    // @param type 陦ｨ遉ｺ繧ｿ繧､繝・//
    void SetDisplayType(int type){ m_nDisplayType = type; }

    // 繧ｯ繧ｨ繧ｹ繝育分蜿ｷ繧定ｨｭ螳壹☆繧・
    // @param quest 繧ｯ繧ｨ繧ｹ繝育分蜿ｷ //
    void SetQuest(int quest){ m_questNum = quest; }

private:

    void Draw() override;

    // 謫堺ｽ懆ｪｬ譏散I繧呈緒逕ｻ縺吶ｋ //
    void OperationDraw() const;

    // 逶ｮ讓呵｡ｨ遉ｺUI繧呈緒逕ｻ縺吶ｋ //
    void TargetDraw() const;

    // 陦ｨ遉ｺ繧ｿ繧､繝怜ｮ壽焚 //
    enum
    {
        Move = 0,     // 遘ｻ蜍墓桃菴・//
        Suction = 1,  // 蜷ｸ縺・ｾｼ縺ｿ謫堺ｽ・//
    };

    int m_nDisplayType;       // 迴ｾ蝨ｨ縺ｮ陦ｨ遉ｺ繧ｿ繧､繝・//
    int m_questNum;           // 迴ｾ蝨ｨ縺ｮ繧ｯ繧ｨ繧ｹ繝育分蜿ｷ //
    std::unique_ptr<CSpriteImage> m_pImage;   // UI逕ｻ蜒・//
};

