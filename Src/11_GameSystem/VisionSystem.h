#pragma once
#include "../05_CommonFile/Object3D.h"

class CVisionSystem : public Object3D
{
public:
    CVisionSystem();
    ~CVisionSystem();
    void Update() override;
    
    bool SectorCircleCollision(const VECTOR2& sectorCenter, const float& humanAngle);

private:
    bool IsAngleInSector(const float& angle) const;
    void SetSectorAngles(const float& centerAngle);
    bool IsPointInSector(const VECTOR2& sectorCenter) const;
    bool LineSegmentCircleIntersection(const VECTOR2& start, const VECTOR2& end, float radius) const;
   


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
        void SetAngle(const XMMATRIX& mat)
        {
            startAngle = (-20 * DegToRad);
        }
    };

    SectorInfo m_sectorInfo;
public:
    CircleInfo GetCircleInfo() const { return m_circleInfo; }
    SectorInfo GetSectorInfo() const { return m_sectorInfo; }
    void SetCircleCenter(const VECTOR3& pos) { m_circleInfo.SetCenter(pos); }
    void SetCircleRadius(const float& radius) { m_circleInfo.SetRadius(radius); }
};
