#pragma once

#include "FunShape.h"
#include "../../Common/Object3D.h"
#include "../Base/EnemyBase.h"


class CHumanBase;
class CHuman : public CEnemyBase
{
public:
    CHuman(const VECTOR3& pos, const VECTOR2& areaSize = VECTOR2(10, 10));
    ~CHuman();
    
    void AddPos(const VECTOR3& pos){ transform.position = transform.position + pos;}
    void SetAngle(float a){angle = a;}
    bool GetInSight() const{return m_inSight;}

private:
    void Update() override;
    void Draw() override;
    ///範囲を描画
   // void DrawDirectionLine();
    void AtkArea() const;
    void FanShape();
    ///
private:
    CFunShape* m_pFunShape;
    DWORD m_dwColor;
    VECTOR2 m_AreaSize;
    float angle;
    bool m_inSight;
};
