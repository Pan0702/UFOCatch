#pragma once
#include "../Common/Object3D.h"

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

    // カットイン情報取得
    int GetCutInNum() const { return m_cutInNum; }
    bool IsCutInVisible() const { return m_isCutInVisible; }

    // ゲームが開始しているか（Go表示後） //
    bool IsGameStarted() const { return m_stareFlag; }

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
    int m_cutInCnt;
    bool m_stareFlag;
    bool m_finishFlag;

    // カットイン用
    int m_cutInNum;           // 0=Ready, 1=Go, 2=Finish
    bool m_isCutInVisible;
    float m_cutInTimer;
};