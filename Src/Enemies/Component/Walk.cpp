#include "Walk.h"
#include "../Base/EnemyBase.h"
#include "../Human/Human.h"
#include "../AnimalSheep/Sheep.h"
#include "../System/Flog.h"

CWalk::CWalk(CEnemyBase* e, float speed)
{
    m_pOwner = e;
    m_moveSpeed = speed;
}

void CWalk::Enter()
{
    m_isFinish = false;
    
    m_totalPosZMoveAmount = 0.0f;
    // 迴ｾ蝨ｨ縺ｮ Transform 繧貞叙蠕・
    Transform transform = m_pOwner->GetTransform();
    m_position = transform.position;

    bool foundValidMove = CalcRandomMove();
    
    // 譛蠕後∪縺ｧ隕九▽縺九ｉ縺ｪ縺九▲縺溷ｴ蜷茨ｼ・
    // 縲檎ｧｻ蜍輔＠縺ｪ縺・ｼ・・峨阪↓縺励※縲∝屓霆｢縺縺代・繝ｩ繝ｳ繝繝縺ｫ荳弱∴繧・
    if (!foundValidMove)
    {
        m_turnAmount = Randomf(-kTurnAngleDeg, kTurnAngleDeg) * DegToRad;
    }

    m_currentRotation = transform.rotation.y;

    //逶ｮ讓吝屓霆｢
    m_targetRotation = m_currentRotation + m_turnAmount;

    //蝗櫁ｻ｢陬憺俣繧定ｵｰ繧峨○繧九◆繧√・繝輔Λ繧ｰ
    m_rotation = true;
    PlayWalkAnimation();
}

/// 蠅・阜蜀・↓蜿弱∪繧九Λ繝ｳ繝繝縺ｪ蝗櫁ｻ｢驥上→遘ｻ蜍戊ｷ晞屬繧・逕滓・縺吶ｋ
/// 繝ｩ繝ｳ繝繝縺ｫ蝗櫁ｻ｢驥擾ｼ・180ﾂｰ・・180ﾂｰ・峨→遘ｻ蜍戊ｷ晞屬  ・・ .0・・.5・峨ｒ逕滓・縺励・蠅・阜繝√ぉ繝・け縺ｫ騾壹ｋ縺ｾ縺ｧ譛螟ｧ50蝗槭Μ繝医Λ繧､縺吶ｋ縲・
/// 譛牙柑縺ｪ邨・∩蜷医ｏ縺帙′隕九▽縺九▲縺溷ｴ蜷医［_turnAm  ount縺ｨm_moveAmount縺ｫ險ｭ螳壹＆繧後ｋ縲・
/// @return 譛牙柑縺ｪ遘ｻ蜍輔ヱ繝ｩ繝｡繝ｼ繧ｿ縺瑚ｦ九▽縺九▲縺溷ｴ  蜷・rue縲・譛螟ｧ隧ｦ陦悟屓謨ｰ繧定ｶ・∴縺溷ｴ蜷・alse
bool CWalk::CalcRandomMove()
{
    static constexpr int kMaxRetry = 50;// 繝ｩ繝ｳ繝繝遘ｻ蜍輔・隧ｦ陦悟屓謨ｰ荳企剞・亥｢・阜螟悶↓蜃ｺ縺ｪ縺・ｵ・∩蜷医ｏ縺帙′隕九▽縺九ｋ縺ｾ縺ｧ譛螟ｧ N 蝗櫁ｩｦ縺呻ｼ・
    static constexpr float kMinMove = 1.0f;// 繝ｩ繝ｳ繝繝遘ｻ蜍戊ｷ晞屬縺ｮ遽・峇・域怙蟆擾ｽ樊怙螟ｧ・・
    static constexpr float kMaxMove = 3.5f;// 繝ｩ繝ｳ繝繝遘ｻ蜍戊ｷ晞屬縺ｮ遽・峇・域怙蟆擾ｽ樊怙螟ｧ・・

    // Sheep蟆ら畑縺ｮ遽・峇繝√ぉ繝・け
    CSheep* sheep = dynamic_cast<CSheep*>(m_pOwner);
    CFlog* flog = nullptr;
    if (sheep != nullptr)
    {
        flog = ObjectManager::FindGameObject<CFlog>();
    }

    // 繝ｩ繝ｳ繝繝縺ｫ・亥屓霆｢驥擾ｼ狗ｧｻ蜍戊ｷ晞屬・峨ｒ菴懊▲縺ｦ縲∝｢・阜蜀・↓蜿弱∪繧九∪縺ｧ繝ｪ繝医Λ繧､
    for (int retry = 0; retry < kMaxRetry; ++retry)
    {
        // 蝗櫁ｻ｢驥擾ｼ喙-180ﾂｰ, +180ﾂｰ] 繧偵Λ繝ｳ繝繝縺ｫ驕ｸ繧薙〒繝ｩ繧ｸ繧｢繝ｳ縺ｸ螟画鋤
        m_turnAmount = Randomf(-kTurnAngleDeg, kTurnAngleDeg) * DegToRad;

        // 遘ｻ蜍戊ｷ晞屬・喙1.0, 3.5] 繧偵Λ繝ｳ繝繝縺ｫ驕ｸ縺ｶ
        m_moveAmount = Randomf(kMinMove, kMaxMove);

        VECTOR3 tmpPos = m_position + VECTOR3(0, 0,
                                      m_moveAmount) * XMMatrixRotationY(m_turnAmount);

        // Sheep縺ｮ蝣ｴ蜷医：log縺ｮ遽・峇蜀・°繝√ぉ繝・け
        if (flog != nullptr)
        {
            VECTOR3 toCenter = flog->GetFlockCenter() - tmpPos;
            toCenter.y = 0;
            float distanceToCenter = sqrtf(toCenter.LengthSquare());
            if (distanceToCenter <= flog->GetFlockRadius())
            {
                return true;
            }
        }
        // 莉悶・繧ｨ繝阪Α繝ｼ縺ｮ蝣ｴ蜷医∵里蟄倥・AreaSize繝√ぉ繝・け
        else if (IsInsideAreaXZ(tmpPos, m_pOwner->GetAreaSize()))
        {
            return true;
        }
    }
    return false;
}
//蛻･縺ｮ縺ｨ縺薙ｍ縺ｫ譖ｸ縺・◆髢｢謨ｰ縺ｧ蜍輔￥縺玖ｩｦ縺吶◆繧√さ繝｡繝ｳ繝医い繧ｦ繝・
// /// 蝗櫁ｻ｢繝ｻ遘ｻ蜍募ｾ後・菴咲ｽｮ縺悟｢・阜蜀・↓蜿弱∪繧九°繝√ぉ繝・け
// /// @param areaSize 繧ｨ繝ｪ繧｢縺ｮ繧ｵ繧､繧ｺ
// /// @return 蠅・阜蜀・↑繧液rue縲∝｢・阜螟悶↑繧映alse
// bool CWalk::BoundaryCheck(const VECTOR2&
//     areaSize) const
// {
//     VECTOR3 tmpPos = m_position + VECTOR3(0, 0,
//                                           m_moveAmount) * XMMatrixRotationY(m_turnAmount);
//     if (tmpPos.x <= areaSize.x && tmpPos.x >=
//         -areaSize.x && tmpPos.z <= areaSize.y &&
//         tmpPos.z >= -areaSize.y)
//     {
//         return true;
//     }
//     return false;
// }

void CWalk::PlayWalkAnimation()
{
    Animator* animator = m_pOwner->GetAnimator();
    animator->MergePlay(AnimationType::A_WALK);
    animator->SetPlaySpeed(1.0f);
}


void CWalk::Update()
{
    if (m_rotation)
    {
        static constexpr float ROTATION_LERP_SPEED = 10.0f;
        float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
        m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
        if (abs(m_targetRotation - m_currentRotation) < 0.01f)
        {
            m_currentRotation = m_targetRotation;
            m_rotation = false;
        }
        m_pOwner->SetRotateY(ClampRotateY(m_currentRotation));
    }

    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(m_currentRotation);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    m_pOwner->AddPosition(moveVec);
    m_totalPosZMoveAmount += m_moveSpeed * SceneManager::DeltaTime();

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        m_isFinish = true;
    }
    if (m_pOwner->IsHuman())
    {
        m_pOwner->IsSuctionCheck();
    }
}

float CWalk::ClampRotateY(float angle)
{
    return std::remainder(angle, XM_2PI);
}
