#pragma once
#include "../Common/Object3D.h"

// 隕也阜蛻､螳壹す繧ｹ繝・Β繧ｯ繝ｩ繧ｹ・域援蠖｢縺ｨ蜀・・陦晉ｪ∝愛螳壹ｒ邂｡逅・ｼ・//
class CVisionSystem : public Object3D
{
public:
    // 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ //
    CVisionSystem();

    // 繝・せ繝医Λ繧ｯ繧ｿ //
    ~CVisionSystem();

    // 謖・ｮ壹＆繧後◆隗貞ｺｦ縺梧援蠖｢縺ｮ隕夜㍽隗貞・縺ｫ縺ゅｋ縺句愛螳・
    // @param angle 蛻､螳壹☆繧玖ｧ貞ｺｦ・医Λ繧ｸ繧｢繝ｳ・・
    // @return 隗貞ｺｦ縺瑚ｦ夜㍽隗貞・縺ｪ繧液rue縲∫ｯ・峇螟悶↑繧映alse //
    bool IsAngleInSector(const float& angle);

    // 謇・ｽ｢縺ｨ蜀・・陦晉ｪ∝愛螳夲ｼ井ｺｺ髢薙・隕也阜遽・峇蜀・↓繝励Ξ繧､繝､繝ｼ縺後＞繧九°繝√ぉ繝・け・・
    // @param humanPos 莠ｺ髢薙・菴咲ｽｮ・・D繝吶け繝医Ν・・
    // @param humanAngle 莠ｺ髢薙・蜷代″・医Λ繧ｸ繧｢繝ｳ・・
    // @return 蜀・′隕也阜謇・ｽ｢蜀・√∪縺溘・謇・ｽ｢縺ｮ蠅・阜縺ｨ莠､蟾ｮ縺励※縺・ｋ蝣ｴ蜷・rue縲√◎繧御ｻ･螟悶・false //
    bool SectorCircleCollision(const VECTOR2& humanPos, float humanAngle);

private:
    // 邱壼・縺ｨ蜀・・莠､蟾ｮ蛻､螳夲ｼ域援蠖｢縺ｮ霎ｺ縺ｨ蜀・′莠､繧上▲縺ｦ縺・ｋ縺九メ繧ｧ繝・け・・
    // @param lineStart 邱壼・縺ｮ髢句ｧ狗せ
    // @param lineEnd 邱壼・縺ｮ邨ゆｺ・せ
    // @param circleCenter 蜀・・荳ｭ蠢・ｺｧ讓・
    // @param circleRadius 蜀・・蜊雁ｾ・
    // @return 邱壼・縺ｨ蜀・′莠､蟾ｮ縺励※縺・ｋ蝣ｴ蜷・rue縲√◎繧御ｻ･螟悶・false //
    bool LineSegmentCircleIntersection(const VECTOR2& lineStart, const VECTOR2& lineEnd, const VECTOR2& circleCenter, float circleRadius) const;
    
    // 蜀・・諠・ｱ繧剃ｿ晄戟縺吶ｋ讒矩菴・//
    struct CircleInfo
    {
        VECTOR2 center;  // 蜀・・荳ｭ蠢・ｺｧ讓・//
        float radius;    // 蜀・・蜊雁ｾ・//

        CircleInfo(const VECTOR2& pos_ = VECTOR2(0,0),const float& radius_ = 0.0f)
            : center(pos_),radius(radius_)
        {}

        // 3D蠎ｧ讓吶°繧厩Z蟷ｳ髱｢縺ｮ2D蠎ｧ讓吶↓螟画鋤縺励※荳ｭ蠢・ｺｧ讓吶ｒ險ｭ螳・
        // @param pos_ 3D蠎ｧ讓・//
        void SetCenter(const VECTOR3& pos_) { center = ToVec2XZ(pos_); }

        // 蜀・・蜊雁ｾ・ｒ險ｭ螳・
        // @param radius_ 險ｭ螳壹☆繧句濠蠕・//
        void SetRadius(const float& radius_) { radius = radius_; }
    };

    CircleInfo m_circleInfo;  // 繝励Ξ繧､繝､繝ｼ縺ｮ蜀・ュ蝣ｱ //

    // 謇・ｽ｢縺ｮ諠・ｱ繧剃ｿ晄戟縺吶ｋ讒矩菴・//
    struct SectorInfo
    {
        float radius;       // 謇・ｽ｢縺ｮ蜊雁ｾ・ｼ郁ｦ也阜霍晞屬・・///Rad/// //
        float startAngle;   // 謇・ｽ｢縺ｮ髢句ｧ玖ｧ貞ｺｦ・医Λ繧ｸ繧｢繝ｳ・・//
        float endAngle;     // 謇・ｽ｢縺ｮ邨ゆｺ・ｧ貞ｺｦ・医Λ繧ｸ繧｢繝ｳ・・//

        SectorInfo()
            : radius(7.0f),startAngle(0),endAngle(0)  // 繝・ヵ繧ｩ繝ｫ繝郁ｦ也阜霍晞屬7.0f //
        {}

        // 謇・ｽ｢縺ｮ蜊雁ｾ・ｒ險ｭ螳・
        // @param radius_ 險ｭ螳壹☆繧句濠蠕・//
        void SetRadius(const float& radius_) { radius = radius_; }
    };

    SectorInfo m_sectorInfo;  // 莠ｺ髢薙・謇・ｽ｢隕也阜諠・ｱ //
public:
    // 蜀・ュ蝣ｱ繧貞叙蠕・
    // @return 蜀・・諠・ｱ //
    CircleInfo GetCircleInfo() const { return m_circleInfo; }

    // 謇・ｽ｢諠・ｱ繧貞叙蠕・
    // @return 謇・ｽ｢縺ｮ諠・ｱ //
    SectorInfo GetSectorInfo() const { return m_sectorInfo; }

    // 蜀・・荳ｭ蠢・ｺｧ讓吶ｒ險ｭ螳・
    // @param pos 險ｭ螳壹☆繧・D蠎ｧ讓・//
    void SetCircleCenter(const VECTOR3& pos) { m_circleInfo.SetCenter(pos); }

    // 蜀・・蜊雁ｾ・ｒ險ｭ螳・
    // @param radius 險ｭ螳壹☆繧句濠蠕・//
    void SetCircleRadius(const float& radius) { m_circleInfo.SetRadius(radius); }
};

