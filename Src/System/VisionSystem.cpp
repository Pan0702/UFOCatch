#include "VisionSystem.h"

#include "../Player/PlayerHP.h"
#include "../Player/Player.h"


CVisionSystem::CVisionSystem() = default;
CVisionSystem::~CVisionSystem() = default;

bool CVisionSystem::IsAngleInSector(const float& angle) 
{
    
    if (m_sectorInfo.startAngle <= m_sectorInfo.endAngle)
    {
        return angle >= m_sectorInfo.startAngle && angle <= m_sectorInfo.endAngle;
    }
    else
    {
        // ・ｽ・ｽ`・ｽ・ｽ0・ｽx・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽ鼾・
        return angle >= m_sectorInfo.startAngle || angle <= m_sectorInfo.endAngle;
    }
}

bool CVisionSystem::LineSegmentCircleIntersection(const VECTOR2& lineStart, 
                                                 const VECTOR2& lineEnd,
                                                 const VECTOR2& circleCenter, 
                                                 float circleRadius) const
{
    VECTOR2 lineDirection = lineEnd - lineStart;
    VECTOR2 startToCenter = lineStart - circleCenter;
    
    float quadraticA = lineDirection.x * lineDirection.x + lineDirection.y * lineDirection.y;
    float quadraticB = 2.0f * (startToCenter.x * lineDirection.x + startToCenter.y * lineDirection.y);
    float quadraticC = (startToCenter.x * startToCenter.x + startToCenter.y * startToCenter.y) - 
                       circleRadius * circleRadius;
    
    float discriminant = quadraticB * quadraticB - 4.0f * quadraticA * quadraticC;
    
    if (discriminant < 0.0f)
    {
        return false;
    }
    
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t1 = (-quadraticB - sqrtDiscriminant) / (2.0f * quadraticA);
    float t2 = (-quadraticB + sqrtDiscriminant) / (2.0f * quadraticA);
    
    return (t1 >= 0.0f && t1 <= 1.0f) || 
           (t2 >= 0.0f && t2 <= 1.0f) || 
           (t1 < 0.0f && t2 > 1.0f);
}
bool CVisionSystem::SectorCircleCollision(const VECTOR2& humanPos, float humanAngle)
{
    // 繝・ヰ繝・げ逕ｨ
    CPlayerHP* pHP = ObjectManager::FindGameObject<CPlayerHP>();
    bool damageFlag = false;
    if (pHP != nullptr)
    {
        damageFlag = pHP->GetFoundFlag();
    }
    
    // m_damage縺荊rue縺ｮ譎ゑｼ育┌謨ｵ譎る俣荳ｭ・峨・遽・峇螟悶→縺励※謇ｱ縺・
    if (pHP && damageFlag)
    {
        return false;
    }

    // 隕夜㍽隗偵ｒ菴ｿ縺｣縺ｦ謇・ｽ｢縺ｮ髢句ｧ九・邨ゆｺ・ｧ貞ｺｦ繧定ｨｭ螳・
    float fieldOfView = 40.0f;  // 隕夜㍽隗抵ｼ亥ｺｦ・・
    float halfFOV = (fieldOfView * 0.5f) * DegToRad;

    m_sectorInfo.startAngle = humanAngle - halfFOV;
    m_sectorInfo.endAngle = humanAngle + halfFOV;
    
    // 謇・ｽ｢縺ｮ荳ｭ蠢・ｼ・uman縺ｮ菴咲ｽｮ・峨°繧牙・縺ｮ荳ｭ蠢・∈縺ｮ繝吶け繝医Ν
    VECTOR2 diff = m_circleInfo.center - humanPos;
    float distSquared = diff.x * diff.x + diff.y * diff.y;
    
    // 譌ｩ譛溘Μ繧ｿ繝ｼ繝ｳ
    float sumRadius = m_sectorInfo.radius + m_circleInfo.radius;
    if (distSquared > sumRadius * sumRadius)
    {
        return false;
    }
    
    // 蜀・・荳ｭ蠢・′謇・ｽ｢蜀・
    if (distSquared <= Pow2(m_sectorInfo.radius))
    {
        float angle = std::atan2(diff.x, diff.y);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    // 蜀・′謇・ｽ｢縺ｮ蜀・ｼｧ縺ｨ莠､蟾ｮ
    if (distSquared < Pow2(m_sectorInfo.radius + m_circleInfo.radius) && 
        distSquared > Pow2(std::abs(m_sectorInfo.radius - m_circleInfo.radius)))
    {
        float angle = std::atan2(diff.x, diff.y);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    //  謇・ｽ｢縺ｮ霎ｺ縺ｨ蜀・・莠､蟾ｮ
    VECTOR2 edge1End = humanPos + VECTOR2(
        std::sin(m_sectorInfo.startAngle) * m_sectorInfo.radius,
        std::cos(m_sectorInfo.startAngle) * m_sectorInfo.radius
    );
    
    VECTOR2 edge2End = humanPos + VECTOR2(
        std::sin(m_sectorInfo.endAngle) * m_sectorInfo.radius,
        std::cos(m_sectorInfo.endAngle) * m_sectorInfo.radius
    );
    
    if (LineSegmentCircleIntersection(humanPos, edge1End, m_circleInfo.center, m_circleInfo.radius) ||
        LineSegmentCircleIntersection(humanPos, edge2End, m_circleInfo.center, m_circleInfo.radius))
    {
        return true;
    }
    
    // 謇・ｽ｢縺ｮ荳ｭ蠢・′蜀・・
    if (distSquared <= m_circleInfo.radius * m_circleInfo.radius)
    {
        return true;
    }
    
    return false;
}
