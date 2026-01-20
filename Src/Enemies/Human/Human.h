#pragma once
#include <unordered_map>

#include "FunShape.h"
#include "../../Common/Object3D.h"
#include "../Base/EnemyBase.h"
#include "State/HumanState.h"

class CHumanBase;
class CHuman : public CEnemyBase
{
public:
    CHuman(VECTOR3 pos,VECTOR2 areaSize = VECTOR2(10, 10));
    ~CHuman();
    
    void AddPos(const VECTOR3& pos){ transform.position = transform.position + pos;}
    VECTOR2 GetAreaSize() const{return m_AreaSize;}
    void SetAngle(float a){angle = a;}
    bool GetInSight() const{return m_inSight;}
    void SetRotateY(float r){transform.rotation.y = r;}

private:
    void CollisionCheck();
    void Update() override;
    void Draw() override;
    ///範囲を描画
    void DrawDirectionLine();
    void AtkArea() const;
    ///
private:
    CFunShape* m_pFunShape;
    DWORD m_dwColor;
    VECTOR2 m_AreaSize;
    float angle;
    bool m_inSight;
};
