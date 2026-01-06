#pragma once
#include <unordered_map>
#include "05_CommonFile/Object3D.h"

class CGameInstance : public Object3D
{
public:
    CGameInstance();
    void AddScore(int score){m_score += score;}
    int GetScore(){return m_score;}
    void Init(int max);
    int GetMaxScore(){return m_maxScore;}
    int GetDiscovery(){return m_discovery;}
    int GetSaw(){return m_saw;}
    void AddDiscovery(int dis){m_discovery += dis;}
    void AddSaw(int saw){m_saw += saw;}

private:
    int m_score;
    int m_maxScore;
    int m_saw;
    int m_discovery;
    
};
