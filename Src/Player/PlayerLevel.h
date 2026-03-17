#pragma once
#include "../Common/Object3D.h"
#include "../Utils/LerpValue.h"

// EXP繝ｻLV繝ｻ蜷ｸ縺・ｾｼ縺ｿ繧ｳ繝ｼ繝ｳ縺ｮ繧ｵ繧､繧ｺ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ //
class CPlayerLevel : public Object3D
{
public:
    CPlayerLevel(float initialConeTopPos, float coneDegree);
    CPlayerLevel(){}
    ~CPlayerLevel() = default;

    void AddExp(float exp) { m_exp += exp; }
    float GetExp()         const { return m_exp; }
    float GetAllExp()      const { return m_allExp; }
    int   GetLv()          const { return m_lv; }
    float GetRadius()      const { return m_coneRadius; }
    float GetConeTopPos()  const { return m_coneTopPos; }
    float GetConeDegree()  const { return m_coneDegree; }

private:
    void Update() override;
    void Draw()   override {}

    void CheckLevel();
    void IncreaseSuctionConeHeight();
    void UpdateHeightAndRadiusLerp();

    float m_exp;
    float m_allExp;
    int   m_lv;

    float m_coneTopPos;
    float m_coneRadius;
    float m_coneDegree;

    LerpValue m_heightLerp;
    LerpValue m_radiusLerp;
};