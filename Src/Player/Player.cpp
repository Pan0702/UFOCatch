#define NOMINMAX
#include "Player.h"
#include "PCamera.h"
#include <algorithm>

#include "ConeDraw.h"
#include "PlayerHP.h"
#include "../System/VisionSystem.h"
#include "../System/Timer.h"

////////////////////
// 原点から移動できる距離
// 例えば-20~20なら20と入力
// @param moveRange 移動可能範囲 //
////////////////////
CPlayer::CPlayer(float moveRange)
    : m_moveRange(moveRange)
{
    transform.position = VECTOR3(0, 5, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Player/UFO.mesh");

    constexpr float INITIAL_CONE_HEIGHT = 4.0f;
    constexpr float CONE_DEGREE = 4.0f;

    m_pLevel = Instantiate<CPlayerLevel>(transform.position.y + INITIAL_CONE_HEIGHT, CONE_DEGREE);

    Instantiate<CPlayerHP>(3);
    Instantiate<CConeDraw>(transform.position.y);
    Instantiate<CCircleDraw>();

    transform.scale = VECTOR3(0.5f, 0.5f, 0.5f);
    m_SuctionActive = false;
    m_prevSuctionActive = false;
    m_zoomUp = false;
    m_draw = true;

    AudioManager::Load("Suction",    _T("data/Sound/suction.wav"));
    AudioManager::Load("SuctionEnd", _T("data/Sound/suctionEnd.wav"));
}

CPlayer::~CPlayer()
{
    SAFE_DELETE(m_pMesh);
}

void CPlayer::Update()
{
    // カメラ位置を更新 //
    UpdateCameraPos();

    // ゲーム開始前は操作を受け付けない
    CTimer* pTimer = ObjectManager::FindGameObject<CTimer>();
    if (pTimer && !pTimer->IsGameStarted())
    {
        return;
    }

    if (!m_SuctionActive)
    {
        HandleMovementInput();
    }

    // 吸引キーの状態を取得
    if (!ObjectManager::FindGameObject<CPlayerHP>()->GetFoundFlag())
    {
        m_SuctionActive = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_J);
    }
    else
    {
        m_SuctionActive = false;
    }

    // 吸引状態が変化したときだけ音声を制御
    if (m_SuctionActive && !m_prevSuctionActive)
    {
        AudioManager::Play(_T("Suction"), false);
    }
    else if (!m_SuctionActive && m_prevSuctionActive)
    {
        AudioManager::Stop(_T("Suction"));
    }

    m_prevSuctionActive = m_SuctionActive;

    CVisionSystem* pVision = ObjectManager::FindGameObject<CVisionSystem>();
    pVision->SetCircleCenter(transform.position);
    pVision->SetCircleRadius(m_pLevel->GetRadius());

    //Debug用
    if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_L))
    {
        transform.scale = VECTOR3(0.3f, 1.0f, 0.f);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_K))
    {
        transform.scale = VECTOR3(0.5f, 0.5f, 0.5f);
    }
}

////////////////////
// プレイヤーの移動入力を処理する //
////////////////////
void CPlayer::HandleMovementInput()
{
    transform.position.x = std::max(-m_moveRange, std::min(m_moveRange, transform.position.x));
    transform.position.z = std::max(-m_moveRange, std::min(m_moveRange, transform.position.z));

    constexpr float moveSpeed = 5.0f;
    const float moveAmount = moveSpeed * SceneManager::DeltaTime();
    auto* input = GameDevice()->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) transform.position.z += moveAmount;
    if (input->CheckKey(KD_DAT, DIK_S)) transform.position.z -= moveAmount;
    if (input->CheckKey(KD_DAT, DIK_A)) transform.position.x -= moveAmount;
    if (input->CheckKey(KD_DAT, DIK_D)) transform.position.x += moveAmount;
}

void CPlayer::UpdateCameraPos()
{
    if (not m_SuctionActive)
    {
        if (m_zoomUp)
        {
            ObjectManager::FindGameObject<CPlayerCamera>()->ZoomOut(transform.position);
            m_zoomUp = false;
        }
        else
        {
            ObjectManager::FindGameObject<CPlayerCamera>()->PosSet(
                transform.position, m_pLevel->GetConeTopPos());
        }
    }
    else
    {
        m_zoomUp = true;
        ObjectManager::FindGameObject<CPlayerCamera>()->ZoomIn(transform.position);
    }
}


////////////////////
// 引き寄せるための移動量を計算する
// 高さの差が大きいほど遠く、近いほど速く吸引する
// @param moveTimeSecond  移動にかける時間
// @param animalPos  動物の位置
// @return 1回当たりの移動量
////////////////////

VECTOR3 CPlayer::CalcSuctionDisplacement(float moveTimeSecond, const VECTOR3& animalPos) const
{
    const float topPos = m_pLevel->GetConeTopPos();
    const float heightDiff = topPos - animalPos.y;
    const float progress = 1.0f - (heightDiff / topPos);
    const float eased = (std::max)(0.0f, (std::min)(1.0f, progress));
    constexpr float minSpeed = 0.4f;                                  
    constexpr float maxSpeed = 1.8f;  
    const float heightSpeedMultiplier = minSpeed + (maxSpeed - minSpeed) * eased;

    const float projectionFactorY0 = avoidZero((0 - animalPos.y) / (animalPos.y - topPos));
    VECTOR3 targetPointOnPlane =
        VECTOR3(animalPos.x + projectionFactorY0 * (animalPos.x - transform.position.x), 0,
                animalPos.z + projectionFactorY0 * (animalPos.z - transform.position.z));
    VECTOR3 pullVectorToTarget = transform.position - targetPointOnPlane;

    const VECTOR3 suctionDisplacementPerFrame = pullVectorToTarget / moveTimeSecond * heightSpeedMultiplier;
    return suctionDisplacementPerFrame * SceneManager::DeltaTime();
}

////////////////////
// オブジェクトがコーンの範囲内にいるかチェックする
// @param targetPos 対象の位置
// @return 範囲内ならtrue //
////////////////////
bool CPlayer::IsWithSuctionCone(const VECTOR3& targetPos) const
{
    const float coneTopPos = m_pLevel->GetConeTopPos();
    const float distanceAnimalFromPlayer = coneTopPos - targetPos.y;
    const float coneRadiusAtTargetHeight = distanceAnimalFromPlayer * std::tan(DegToRad * m_pLevel->GetConeDegree());
    return (Pow2(targetPos.x - transform.position.x) + Pow2(targetPos.z - transform.position.z)
            <= Pow2(coneRadiusAtTargetHeight));
}

bool CPlayer::IsInsideSuctionCircle(const VECTOR3& targetPos) const
{
    const float dx = targetPos.x - transform.position.x;
    const float dz = targetPos.z - transform.position.z;
    const float radius = m_pLevel->GetRadius();
    return (dx * dx + dz * dz) <= (radius * radius);
}


void CPlayer::Draw()
{
    CPlayerHP* hp = ObjectManager::FindGameObject<CPlayerHP>();
    if (hp->GetFoundFlag())
    {
        m_draw = !m_draw;
    }
    else
    {
        m_draw = true;
    }
    if (m_draw)
    {
        Object3D::Draw();
    }
}