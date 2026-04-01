#pragma once
#include "../Common/Object3D.h"

// 繧ｲ繝ｼ繝蜀・・繧ｿ繧､繝槭・繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ //
class CTimer : public Object3D
{
public:
    // 繧ｿ繧､繝槭・縺ｮ譛螟ｧ譎る俣繧呈欠螳壹＠縺ｦ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
    // @param maxTimeSec 譛螟ｧ譎る俣・育ｧ抵ｼ・//
    CTimer(float maxTimeSec);
    ~CTimer();

    float GetTime() {return m_currentTime;}
    void SetStareFlag(bool flag) {m_stareFlag = flag;}

    // 繧ｫ繝・ヨ繧､繝ｳ諠・ｱ蜿門ｾ・
    int GetCutInNum() const { return m_cutInNum; }
    bool IsCutInVisible() const { return m_isCutInVisible; }

    // 繧ｲ繝ｼ繝縺碁幕蟋九＠縺ｦ縺・ｋ縺具ｼ・o陦ｨ遉ｺ蠕鯉ｼ・
    bool IsGameStarted() const { return m_stareFlag; }

private:
    void Update() override;
    // 譎る俣繧呈緒逕ｻ縺吶ｋ
    // @param posX X蠎ｧ讓・
    // @param posY Y蠎ｧ讓・
    // @param num 謨ｰ蛟､ //
    void DrawTime(const float& posX, const float& posY, const float& num) const;
    void TimeOut();

private:
    float m_currentTime;
    float m_maxTime;
    int m_cutInCnt;
    bool m_stareFlag;
    bool m_finishFlag;

    // 繧ｫ繝・ヨ繧､繝ｳ逕ｨ
    int m_cutInNum;           // 0=Ready, 1=Go, 2=Finish
    bool m_isCutInVisible;
    float m_cutInTimer;
};

