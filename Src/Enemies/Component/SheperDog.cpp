#include "SheperDog.h"
#include "../AnimalDog//ShepherdDog.h"  // パスは調整してください
#include "../System/Flog.h"

CCollectiong::CCollectiong(CAShepherdDog* dog,float speed)
    :m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CCollectiong::Enter()
{
    m_isFinish = false;
    
    // 群れ情報を取得
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());
    
    // はぐれ羊の位置
    VECTOR3 sheepPos = info.furthestSheep->GetTransform().position;
    
    // はぐれ羊から重心への方向
    VECTOR3 toCentroid = info.centroid - sheepPos;
    normalize(toCentroid);
    
    static constexpr float m_behindDistance = 3.0f;
    // 背後に回り込む位置を計算
    m_targetPos = sheepPos - toCentroid * m_behindDistance;
}

void CCollectiong::Update()
{
    // 目標位置へ移動
    VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    float distance = direction.LengthSquare();
    
    // 目標に到達したら終了
    if (distance < 0.5f) {
        m_isFinish = true;
        return;
    }
    
    normalize(direction);
    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}

CDriving::CDriving(CAShepherdDog* dog,float speed)
    :m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CDriving::Enter()
{
    m_isFinish = false;
    
    // 群れ情報を取得
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());
    
    // UFO（Player）の位置を取得
    VECTOR3 ufoPos = ObjectManager::FindGameObject<CPlayer>()->GetTransform().position;
    VECTOR3 centroid = info.centroid;
    
    // UFOから群れへの方向（逃げる方向）
    VECTOR3 escapeDir = centroid - ufoPos;
    normalize(escapeDir);
    
    // 群れとUFOの間に割り込む位置
    m_targetPos = centroid - escapeDir * m_interceptDistance;
}

void CDriving::Update()
{
    // 目標位置へ移動
    VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    float distance = direction.LengthSquare();
    
    // 目標に到達したら終了
    if (distance < 0.5f) {
        m_isFinish = true;
        return;
    }
    
    normalize(direction);
    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}