#pragma once
#include <unordered_map>

#include "AnimalManager.h"
#include "../../05_CommonFile/Object3D.h"


class CHuman : public CAnimalManager
{
public:
    CHuman();
    ~CHuman();
    void SetState(CBaseState<CHuman>::Type type);
    
    void AddPos(const VECTOR3& pos){ transform.position = transform.position + pos;}
    VECTOR2 GetAreaSize() const{return m_AreaSize;}
    void AddAngle(float a){angle = a;}

private:
    void Update() override;
    void Draw() override;
    ///範囲を描画
    void DrawDirectionLine();
    void FanShape();
    ///
private:
    std::unordered_map<CBaseState<CHuman>::Type, CBaseState<CHuman>*> m_cubeStates;
    CBaseState<CHuman>* m_pCurrentState;
    DWORD m_dwColor;
    VECTOR2 m_AreaSize;
    float angle;
};
