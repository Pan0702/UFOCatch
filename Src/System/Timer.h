#pragma once
#include "../Common/Object3D.h"

// ゲーム内のタイマーを管理するクラス //
class CTimer : public Object3D
{
public:
    // タイマーの最大時間を指定してインスタンスを生成する
    // @param maxTimeSec 最大時間（秒） //
    /// CTimer を初期化する
    /// @param maxTimeSec maxTimeSec に渡す値
    CTimer(float maxTimeSec);
    /// CTimer の終了処理を行う
    ~CTimer();

    /// Time を取得する
    /// @return 計算結果の値
    float GetTime() { return m_currentTime; }
    /// Stare Flag を設定する
    /// @param flag flag に渡す値
    void SetStareFlag(bool flag) { m_stareFlag = flag; }

    // カットイン情報取得
    /// Cut In Num を取得する
    /// @return 処理結果の数値
    int GetCutInNum() const { return m_cutInNum; }
    /// Cut In Visible を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsCutInVisible() const { return m_isCutInVisible; }

    // ゲームが開始しているか（Go表示後） //
    /// Game Started を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsGameStarted() const { return m_stareFlag; }

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    // 時間を描画する
    // @param posX X座標
    // @param posY Y座標
    // @param num 数値 //
    /// Time を描画する
    /// @param posX 座標
    /// @param posY 座標
    /// @param num num に渡す値
    void DrawTime(const float& posX, const float& posY, const float& num) const;
    /// Time Out の処理を行う
    void TimeOut();

private:
    float m_currentTime;
    float m_maxTime;
    int m_cutInCnt;
    bool m_stareFlag;
    bool m_finishFlag;

    // カットイン用
    int m_cutInNum; // 0=Ready, 1=Go, 2=Finish
    bool m_isCutInVisible;
    float m_cutInTimer;
};
