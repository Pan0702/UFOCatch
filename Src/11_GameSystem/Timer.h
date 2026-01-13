#pragma once
#include "../05_CommonFile/Object3D.h"

// ゲーム内のタイマーを管理するクラス //
class CTimer : public Object3D
{
public:
    // タイマーの最大時間を指定してインスタンスを生成する
    // @param maxTimeSec 最大時間（秒） //
    CTimer(float maxTimeSec);
    ~CTimer();

    float GetTime() {return m_currentTime;}
    void SetStareFlag(bool flag) {m_stareFlag = flag;}

private:
    void Update() override;
    // 時間を描画する
    // @param posX X座標
    // @param posY Y座標
    // @param num 数値 //
    void DrawTime(const float& posX, const float& posY, const float& num) const;
    void TimeOut();

private:
    float m_currentTime;
    float m_maxTime;
    bool m_stareFlag;
};
