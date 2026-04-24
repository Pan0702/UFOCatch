#include "VisionSystem.h"

#include "../Player/PlayerHP.h"
#include "../Player/Player.h"


CVisionSystem::CVisionSystem() = default;
CVisionSystem::~CVisionSystem() = default;

bool CVisionSystem::IsAngleInSector(const float& angle) const
{
    if (m_sectorInfo.startAngle <= m_sectorInfo.endAngle)
    {
        return angle >= m_sectorInfo.startAngle && angle <= m_sectorInfo.endAngle;
    }
    else
    {
        // 角度が 0度をまたぐ場合の処理
        return angle >= m_sectorInfo.startAngle || angle <= m_sectorInfo.endAngle;
    }
}

bool CVisionSystem::LineSegmentCircleIntersection(const VECTOR2& lineStart, const VECTOR2& lineEnd,
                                                 const VECTOR2& circleCenter,float circleRadius) const
{
    const VECTOR2 lineDirection = lineEnd - lineStart;
    const VECTOR2 startToCenter = lineStart - circleCenter;
    
    const float quadraticA = lineDirection.x * lineDirection.x + lineDirection.y * lineDirection.y;
    const float quadraticB = 2.0f * (startToCenter.x * lineDirection.x + startToCenter.y * lineDirection.y);
    const float quadraticC = (startToCenter.x * startToCenter.x + startToCenter.y * startToCenter.y) - 
                       circleRadius * circleRadius;
    
    const float discriminant = quadraticB * quadraticB - 4.0f * quadraticA * quadraticC;
    
    if (discriminant < 0.0f)
    {
        return false;
    }
    
    const float sqrtDiscriminant = std::sqrt(discriminant);
    const float t1 = (-quadraticB - sqrtDiscriminant) / (2.0f * quadraticA);
    const float t2 = (-quadraticB + sqrtDiscriminant) / (2.0f * quadraticA);
    
    return (t1 >= 0.0f && t1 <= 1.0f) || 
           (t2 >= 0.0f && t2 <= 1.0f) || 
           (t1 < 0.0f && t2 > 1.0f);
}

bool CVisionSystem::SectorCircleCollision(const VECTOR2& humanPos, float humanAngle)
{
    // デバッグ用
    CPlayerHP* pHP = ObjectManager::FindGameObject<CPlayerHP>();
    bool damageFlag = false;
    if (pHP != nullptr)
    {
        damageFlag = pHP->GetFoundFlag();
    }
    
    // damageフラグがtrueの時（無敵時間中など）は範囲外として扱う
    if (pHP && damageFlag)
    {
        return false;
    }

    // 視野角を使って扇形の開始・終了角度を設定
    constexpr float fieldOfView = 40.0f;  // 視野角（度）
    constexpr float halfFOV = (fieldOfView * 0.5f) * DegToRad;

    m_sectorInfo.startAngle = humanAngle - halfFOV;
    m_sectorInfo.endAngle = humanAngle + halfFOV;
    
    // 扇形の中心（Humanの位置）から円の中心へのベクトル
    const VECTOR2 diff = m_circleInfo.center - humanPos;
    const float distSquared = diff.x * diff.x + diff.y * diff.y;
    
    // 早期リターン（大まかな距離チェック）
    const float sumRadius = m_sectorInfo.radius + m_circleInfo.radius;
    if (distSquared > sumRadius * sumRadius)
    {
        return false;
    }
    
    // 円の中心が扇形内にあるか
    if (distSquared <= Pow2(m_sectorInfo.radius))
    {
        float angle = std::atan2(diff.x, diff.y);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    // 円が扇形の円弧と交差しているか
    if (distSquared < Pow2(m_sectorInfo.radius + m_circleInfo.radius) && 
        distSquared > Pow2(std::abs(m_sectorInfo.radius - m_circleInfo.radius)))
    {
        float angle = std::atan2(diff.x, diff.y);
        if (IsAngleInSector(angle))
        {
            return true;
        }
    }
    
    //  扇形の辺と円の交差
    const VECTOR2 edge1End = humanPos + VECTOR2(
        std::sin(m_sectorInfo.startAngle) * m_sectorInfo.radius,
        std::cos(m_sectorInfo.startAngle) * m_sectorInfo.radius
    );
    
    const VECTOR2 edge2End = humanPos + VECTOR2(
        std::sin(m_sectorInfo.endAngle) * m_sectorInfo.radius,
        std::cos(m_sectorInfo.endAngle) * m_sectorInfo.radius
    );
    
    if (LineSegmentCircleIntersection(humanPos, edge1End, m_circleInfo.center, m_circleInfo.radius) ||
        LineSegmentCircleIntersection(humanPos, edge2End, m_circleInfo.center, m_circleInfo.radius))
    {
        return true;
    }
    
    // 扇形の中心が円内にあるか
    if (distSquared <= m_circleInfo.radius * m_circleInfo.radius)
    {
        return true;
    }
    
    return false;
}


void CVisionSystem::SetCircleCenter(const VECTOR3& pos)
{
    m_circleInfo.SetCenter(pos);
}

void CVisionSystem::SetCircleRadius(const float& radius)
{
    m_circleInfo.SetRadius(radius); 
}