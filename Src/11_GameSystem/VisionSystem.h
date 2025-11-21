#pragma once
#include "../05_CommonFile/Object3D.h"

class CVisionSystem : public Object3D
{
public:
    CVisionSystem();
    ~CVisionSystem();
    bool IsAngleInSector(const float& angle);
    bool SectorCircleCollision(const VECTOR2& humanPos, float humanAngle);

private:
    bool LineSegmentCircleIntersection(const VECTOR2& lineStart, const VECTOR2& lineEnd, const VECTOR2& circleCenter, float circleRadius) const;



    struct CircleInfo
    {
        VECTOR2 center;
        float radius;
        CircleInfo(const VECTOR2& pos_ = VECTOR2(0,0),const float& radius_ = 0.0f)
            : center(pos_),radius(radius_)
        {}
        void SetCenter(const VECTOR3& pos_) { center = ToVec2XZ(pos_); }
        void SetRadius(const float& radius_) { radius = radius_; }
    };

    CircleInfo m_circleInfo;

    struct SectorInfo
    {
        ///Rad///
        float radius;
        float startAngle ;
        float endAngle ;
        SectorInfo()
            : radius(7.0f),startAngle(0),endAngle(0)
        {}
        void SetRadius(const float& radius_) { radius = radius_; }
    };

    SectorInfo m_sectorInfo;
public:
    CircleInfo GetCircleInfo() const { return m_circleInfo; }
    SectorInfo GetSectorInfo() const { return m_sectorInfo; }
    void SetCircleCenter(const VECTOR3& pos) { m_circleInfo.SetCenter(pos); }
    void SetCircleRadius(const float& radius) { m_circleInfo.SetRadius(radius); }
};
