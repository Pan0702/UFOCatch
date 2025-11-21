#include "VisionSystem.h"


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
        // ��`��0�x���܂����ꍇ
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
    // 視野角を使って扇形の開始・終了角度を設定
    float fieldOfView = 40.0f;  // 視野角（度）
    float halfFOV = (fieldOfView * 0.5f) * DegToRad;
    
    m_sectorInfo.startAngle = humanAngle - halfFOV;
    m_sectorInfo.endAngle = humanAngle + halfFOV;
    
    // 扇形の中心（Humanの位置）から円の中心へのベクトル
    VECTOR2 diff = m_circleInfo.center - humanPos;
    float distSquared = diff.x * diff.x + diff.y * diff.y;
    float dist = std::sqrt(distSquared);
    
    // 早期リターン
    float sumRadius = m_sectorInfo.radius + m_circleInfo.radius;
    if (distSquared > sumRadius * sumRadius)
    {
        return false;
    }
    
    // 円の中心が扇形内
    if (dist <= m_sectorInfo.radius)
    {
        float angle = std::atan2(diff.x, diff.y);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    // 円が扇形の円弧と交差
    if (dist < m_sectorInfo.radius + m_circleInfo.radius && 
        dist > std::abs(m_sectorInfo.radius - m_circleInfo.radius))
    {
        float angle = std::atan2(diff.x, diff.y);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    //  扇形の辺と円の交差
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
    
    // 扇形の中心が円内
    if (distSquared <= m_circleInfo.radius * m_circleInfo.radius)
    {
        return true;
    }
    
    return false;
}