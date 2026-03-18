#define NOMINMAX
#include "Player.h"
#include "PCamera.h"
#include <algorithm>

#include "ConeDraw.h"
#include "PlayerHP.h"
#include "../System/VisionSystem.h"
#include "../System/Timer.h"

////////////////////
// 蜴溽せ縺九ｉ遘ｻ蜍輔〒縺阪ｋ霍晞屬
// 萓九∴縺ｰ-20~20縺ｪ繧・0縺ｨ蜈･蜉・
// @param moveRange 遘ｻ蜍募庄閭ｽ遽・峇 //
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
    // 繧ｫ繝｡繝ｩ菴咲ｽｮ繧呈峩譁ｰ//
    UpdateCameraPos();

    // 繧ｲ繝ｼ繝髢句ｧ句燕縺ｯ謫堺ｽ懊ｒ蜿励￠莉倥￠縺ｪ縺・
    CTimer* pTimer = ObjectManager::FindGameObject<CTimer>();
    if (pTimer && !pTimer->IsGameStarted())
    {
        return;
    }

    if (!m_SuctionActive)
    {
        HandleMovementInput();
    }

    // 蜷ｸ縺・ｾｼ縺ｿ繧ｭ繝ｼ縺ｮ迥ｶ諷九ｒ蜿門ｾ・
    if (!ObjectManager::FindGameObject<CPlayerHP>()->GetFoundFlag())
    {
        m_SuctionActive = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_J);
    }
    else
    {
        m_SuctionActive = false;
    }

    // 蜷ｸ縺・ｾｼ縺ｿ迥ｶ諷九′螟牙喧縺励◆縺ｨ縺阪□縺鷹浹螢ｰ繧貞宛蠕｡
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

    //Debug逕ｨ
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
// 繝励Ξ繧､繝､繝ｼ縺ｮ遘ｻ蜍募・蜉帙ｒ蜃ｦ逅・☆繧・//
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
// 蠑輔″蟇・○繧九◆繧√・遘ｻ蜍暮㍼繧定ｨ育ｮ励☆繧・
// 鬮倥＆縺ｮ蟾ｮ縺悟､ｧ縺阪＞縺ｻ縺ｩ驕・￥縲∬ｿ代＞縺ｻ縺ｩ騾溘￥蜷ｸ縺・ｾｼ繧
// @param moveTimeSecond  遘ｻ蜍輔↓縺九￠繧区凾髢・
// @param animalPos  蜍慕黄縺ｮ菴咲ｽｮ
// @return 1蝗槫ｽ薙◆繧翫・遘ｻ蜍暮㍼
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
// 繧ｪ繝悶ず繧ｧ繧ｯ繝医′繧ｳ繝ｼ繝ｳ縺ｮ遽・峇蜀・↓縺・ｋ縺九メ繧ｧ繝・け縺吶ｋ
// @param targetPos 蟇ｾ雎｡縺ｮ菴咲ｽｮ
// @return 遽・峇蜀・↑繧液rue //
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

