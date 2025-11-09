#pragma once
#include <unordered_map>

#include "AnimalManager.h"
#include "../../05_CommonFile/Object3D.h"


class CHuman : public CAnimalManager
{
public:
    CHuman();
    ~CHuman();

private:
    void Update() override;
    void Draw() override;
    void SetState(CBaseState<CHuman>::Type type);
    ///範囲を描画
    void DrawDirectionLine();
    void FanShape();
    ///
private:
    std::unordered_map<CBaseState<CHuman>::Type, CBaseState<CHuman>*> m_cubeStates;
    CBaseState<CHuman>* m_pCurrentState;
    DWORD m_dwColor;
};
