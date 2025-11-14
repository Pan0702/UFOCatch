#pragma once
#include <unordered_map>

#include "AnimalManager.h"
#include "FunShape.h"
#include "../../05_CommonFile/Object3D.h"
#include "../State/HumanState.h"

class CHumanBase;
class CHuman : public CAnimalManager
{
public:
    CHuman();
    ~CHuman();
    void ChangeState(CBaseState::Type type);
    
    void AddPos(const VECTOR3& pos){ transform.position = transform.position + pos;}
    VECTOR2 GetAreaSize() const{return m_AreaSize;}
    void SetAngle(float a){angle = a;}
    bool GetInSight() const{return m_inSight;}

private:
    void Update() override;
    void Draw() override;
    ///範囲を描画
    void DrawDirectionLine();
    void FanShape();
    void AtkArea();
    ///
private:
    std::unordered_map<CBaseState::Type, CHumanBase*> m_cubeStates;
    CBaseState* m_pCurrentState;
    CFunShape* m_pFunShape;
    DWORD m_dwColor;
    VECTOR2 m_AreaSize;
    float angle;
    bool m_inSight;
};
