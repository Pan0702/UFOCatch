#include "VisionSystem.h"


CVisionSystem::CVisionSystem() = default;
CVisionSystem::~CVisionSystem() = default;






bool CVisionSystem::IsAngleInSector(float angle) const
{
    
    if (start <= end)
    {
        return angle >= start && angle <= end;
    }
    else
    {
        // 扇形が0度をまたぐ場合
        return angle >= start || angle <= end;
    }
}

bool CVisionSystem::LineSegmentCircleIntersection(const VECTOR2& lineStart, 
                                                 const VECTOR2& lineEnd,
                                                 const VECTOR2& circleCenter, 
                                                 float circleRadius) const
{
    // 線分の方向ベクトル
    VECTOR2 lineDirection = lineEnd - lineStart;
    
    // 線分の始点から円の中心へのベクトル
    VECTOR2 startToCenter = lineStart - circleCenter;
    
    // 二次方程式 at? + bt + c = 0 の係数
    float quadraticA = lineDirection.x * lineDirection.x + lineDirection.y * lineDirection.y;
    float quadraticB = 2.0f * (startToCenter.x * lineDirection.x + startToCenter.y * lineDirection.y);
    float quadraticC = (startToCenter.x * startToCenter.x + startToCenter.y * startToCenter.y) - 
                       circleRadius * circleRadius;
    
    // 判別式
    float discriminant = quadraticB * quadraticB - 4.0f * quadraticA * quadraticC;
    
    if (discriminant < 0.0f)
    {
        return false;  // 交点なし
    }
    
    // 交点のパラメータt を計算
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t1 = (-quadraticB - sqrtDiscriminant) / (2.0f * quadraticA);
    float t2 = (-quadraticB + sqrtDiscriminant) / (2.0f * quadraticA);
    
    // 交点が線分上（t ∈ [0,1]）にあるかチェック
    return (t1 >= 0.0f && t1 <= 1.0f) || 
           (t2 >= 0.0f && t2 <= 1.0f) || 
           (t1 < 0.0f && t2 > 1.0f);
}

bool CVisionSystem::SectorCircleCollision(const VECTOR2& sectorCenter, 
                                         const VECTOR2& circleCenter) const
{
    // 扇形の中心から円の中心へのベクトル
    VECTOR2 diff = circleCenter - sectorCenter;
    float distSquared = diff.x * diff.x + diff.y * diff.y;
    float dist = std::sqrt(distSquared);
    
    // 早期リターン: 円が扇形の外接円の外側
    float sumRadius = m_sectorInfo.radius + m_circleInfo.radius;
    if (distSquared > sumRadius * sumRadius)
    {
        return false;
    }
    
    // ケース1: 円の中心が扇形内にある
    if (dist <= m_sectorInfo.radius)
    {
        float angle = std::atan2(diff.x, diff.y);  // 注意: atan2(x, y) でZ軸正方向が0度
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    // ケース2: 円が扇形の円弧と交差
    if (dist < m_sectorInfo.radius + m_circleInfo.radius && 
        dist > std::abs(m_sectorInfo.radius - m_circleInfo.radius))
    {
        float angle = std::atan2(diff.x, diff.y);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    // ケース3: 扇形の辺（半径線）と円の交差
    VECTOR2 edge1End = sectorCenter + VECTOR2(
        std::sin(m_sectorInfo.startAngle) * m_sectorInfo.radius,
        std::cos(m_sectorInfo.startAngle) * m_sectorInfo.radius
    );
    
    VECTOR2 edge2End = sectorCenter + VECTOR2(
        std::sin(m_sectorInfo.endAngle) * m_sectorInfo.radius,
        std::cos(m_sectorInfo.endAngle) * m_sectorInfo.radius
    );
    
    if (LineSegmentCircleIntersection(sectorCenter, edge1End, circleCenter, m_circleInfo.radius) ||
        LineSegmentCircleIntersection(sectorCenter, edge2End, circleCenter, m_circleInfo.radius))
    {
        return true;
    }
    
    // ケース4: 扇形の中心が円内にある
    if (distSquared <= m_circleInfo.radius * m_circleInfo.radius)
    {
        return true;
    }
    
    return false;
}