#pragma once
#include <unordered_map>
#include "../Common/Object3D.h"

/// <summary>ゲームシステムで使う Game Instance の情報と処理をまとめる型</summary>
class CGameInstance : public Object3D
{
public:
    /// CGameInstance を初期化する
    CGameInstance();
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 初期化する
    /// @param max max に渡す値
    void Init(int max);

    // Get関数
    /// Max Score を取得する
    /// @return 処理結果の数値
    int GetMaxScore() const { return m_maxScore; }
    /// Score を取得する
    /// @return 処理結果の数値
    int GetScore() const { return m_score; }
    /// Discovery を取得する
    /// @return 処理結果の数値
    int GetDiscovery() const { return m_discovery; }
    /// Capture を取得する
    /// @return 処理結果の数値
    int GetCapture() const { return m_capture; }
    /// Saw を取得する
    /// @return 処理結果の数値
    int GetSaw() const { return m_saw; }
    /// Map Size を取得する
    /// @return 4次元ベクトル
    const VECTOR4& GetMapSize() const { return m_mapSize; }
    /// Map Size を設定する
    /// @param size サイズ
    void SetMapSize(const VECTOR4& size) { m_mapSize = size; }

    // Add関数
    /// Score を追加する
    /// @param score score に渡す値
    void AddScore(int score) { m_score += score; }
    /// Discovery を追加する
    /// @param dis dis に渡す値
    void AddDiscovery(int dis) { m_discovery += dis; }
    /// Capture を追加する
    /// @param cap cap に渡す値
    void AddCapture(int cap) { m_capture += cap; }
    /// Saw を追加する
    /// @param saw saw に渡す値
    void AddSaw(int saw) { m_saw += saw; }

    /// 取得する
    /// @return 対象のポインタ
    static CGameInstance* Get()
    {
        return SingleInstantiate<CGameInstance>();
    }

private:
    int m_score;
    int m_maxScore;
    int m_saw;
    int m_discovery;
    int m_capture;
    VECTOR4 m_mapSize;
};
