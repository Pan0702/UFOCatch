#include "VisionSystem.h"


CVisionSystem::CVisionSystem() = default;
CVisionSystem::~CVisionSystem() = default;


bool CVisionSystem::IsAngleInSector(const float& angle) const
{
    if (m_sectorInfo.startAngle <= angle && angle <= m_sectorInfo.endAngle)
    {
        return true;
    }
    return false;
}
bool CVisionSystem::IsPointInSector( const VECTOR2& sectorCenter) const
{
    VECTOR2 diff = m_circleInfo.center - sectorCenter;
    float distSquared  = Pow2(diff.x) + Pow2(diff.y);

    if ( distSquared > Pow2(m_sectorInfo.radius))
    {
        return false;
    }

    float angle = std::atan2(diff.y, diff.x);
    return IsAngleInSector(angle);
}

bool CVisionSystem::LineSegmentCircleIntersection(const VECTOR2& start, const VECTOR2& end, float radius) const
{
    VECTOR2 lineDirection  = end - start;
    VECTOR2 startToCenter  = start - m_circleInfo.center;

    float quadraticA  = dot(lineDirection, lineDirection);
    float quadraticB  = 2 * dot(lineDirection, startToCenter);
    float quadraticC  = dot(startToCenter, startToCenter) - Pow2(radius);
    float discriminant = Pow2(quadraticB) - 4 * quadraticA * quadraticC;
    if (discriminant < 0.0f)
    {
        return false;
    }

    discriminant = sqrt(discriminant);
    float t1 = (-quadraticB - discriminant) / (2 * quadraticA);
    float t2 = (-quadraticB + discriminant) / (2 * quadraticA);
    return (t1 >= 0.0f && t1 <= 1.0f) || (t2 >= 0.0f && t2 <= 1.0f)||(t1 < 0.0f && t2 > 1.0f);
}

bool CVisionSystem::SectorCircleCollision(const VECTOR2& sectorCenter) const
{
    VECTOR2 diff = m_circleInfo.center - sectorCenter;
    float distSquared  = Pow2(diff.x) + Pow2(diff.y);
    float sumRadius = m_sectorInfo.radius + m_circleInfo.radius;
    if (distSquared > Pow2(sumRadius))
    {
        return false;
    }
    if (IsPointInSector(sectorCenter))
    {
        return true;
    }

    float dist = std::sqrt(distSquared);
    if (dist < m_sectorInfo.radius + m_circleInfo.radius
        && dist >std::abs(m_sectorInfo.radius - m_circleInfo.radius))
    {
        float angle = std::atan2(diff.y, diff.x);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }

    VECTOR2 edgeEnd1 = sectorCenter + 
        VECTOR2(std::cos(m_sectorInfo.startAngle), std::sin(m_sectorInfo.startAngle)) * m_sectorInfo.radius;

    VECTOR2 edgeEnd2 = sectorCenter+ 
        VECTOR2(std::cos(m_sectorInfo.endAngle), std::sin(m_sectorInfo.endAngle)) * m_sectorInfo.radius;
    if (LineSegmentCircleIntersection(sectorCenter, edgeEnd1, m_circleInfo.radius) ||
        LineSegmentCircleIntersection(sectorCenter, edgeEnd2, m_circleInfo.radius))
    {
        return true;
    }

    if (distSquared <= Pow2(m_circleInfo.radius))
    {
        return true;
    }

    return false;
}
