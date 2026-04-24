#pragma once
#include <unordered_map>
#include "../Common/Object3D.h"

class CGameInstance : public Object3D
{
public:
    CGameInstance();
    void Update() override;
    void Init(int max);

    // Get関数
    int GetMaxScore() const { return m_maxScore; }
    int GetScore() const { return m_score; }
    int GetDiscovery() const { return m_discovery; }
    int GetCapture() const { return m_capture; }
    int GetSaw() const{ return m_saw; }
    const VECTOR4& GetMapSize() const { return m_mapSize; }
    void SetMapSize(const VECTOR4& size) { m_mapSize = size; }

    // Add関数
    void AddScore(int score) { m_score += score; }
    void AddDiscovery(int dis) { m_discovery += dis; }
    void AddCapture(int cap) { m_capture += cap; }
    void AddSaw(int saw) { m_saw += saw; }
    
    static CGameInstance* Get() {
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