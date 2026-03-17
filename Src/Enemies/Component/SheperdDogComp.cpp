#include "SheperdDogComp.h"
#include "../AnimalDog//ShepherdDog.h"  // 繝代せ縺ｯ隱ｿ謨ｴ縺励※縺上□縺輔＞
#include "../System/Flog.h"

CCollecting::CCollecting(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CCollecting::Enter()
{
    m_isFinish = false;

    // 鄒､繧梧ュ蝣ｱ繧貞叙蠕・
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());

    // 縺ｯ縺舌ｌ鄒翫′縺・↑縺・ｴ蜷医・邨ゆｺ・
    if (info.furthestSheep == nullptr)
    {
        m_isFinish = true;
        return;
    }

    // 縺ｯ縺舌ｌ鄒翫・菴咲ｽｮ
    const VECTOR3 sheepPos = info.furthestSheep->GetTransform().position;
    // 縺ｯ縺舌ｌ鄒翫°繧蛾㍾蠢・∈縺ｮ譁ｹ蜷・
    VECTOR3 toCentroid = info.centroid - sheepPos;
    toCentroid.y = 0;

    // 繧ｼ繝ｭ繝吶け繝医Ν蟇ｾ遲・
    if (toCentroid.LengthSquare() < 0.0001f)
    {
        m_isFinish = true;
        return;
    }

    normalize(toCentroid);
    // 鄒翫・閭悟ｾ後↓遶九▽霍晞屬・医せ繝医Ο繝ｳ繝懊Β: 5m・・
    constexpr float behindDistance = 5.0f;
    // 閭悟ｾ後↓蝗槭ｊ霎ｼ繧菴咲ｽｮ繧定ｨ育ｮ・
    m_targetPos = sheepPos - toCentroid * behindDistance;
}

void CCollecting::Update()
{
    // 逶ｮ讓吩ｽ咲ｽｮ縺ｸ遘ｻ蜍・
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    direction.y = 0;
    const float distanceSq = direction.LengthSquare();
    constexpr float arrivalThresholdSq = 0.25f;
    // 逶ｮ讓吶↓蛻ｰ驕斐＠縺溘ｉ邨ゆｺ・
    if (distanceSq < arrivalThresholdSq)
    {
        m_isFinish = true;
        return;
    }

    normalize(direction);

    // 遘ｻ蜍墓婿蜷代↓蝗櫁ｻ｢
    float targetAngle = atan2f(direction.x, direction.z);
    m_pOwner->SetRotateY(targetAngle);

    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}

CDriving::CDriving(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CDriving::Enter()
{
    m_isFinish = false;

    // 繝・ヰ繝・げ・夂ｾ翫・謨ｰ繧堤｢ｺ隱・
    size_t sheepCount = m_pOwner->GetSheeps().size();

    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());
    CPlayer* player = ObjectManager::FindGameObject<CPlayer>();
    if (player == nullptr) return;

    VECTOR3 ufoPos = player->GetTransform().position;
    VECTOR3 centroid = info.centroid;

    // UFO縺九ｉ鄒､繧後∈縺ｮ譁ｹ蜷托ｼ磯・￡繧区婿蜷托ｼ・
    VECTOR3 escapeDir = centroid - ufoPos;
    escapeDir.y = 0;

    // 繧ｼ繝ｭ繝吶け繝医Ν蟇ｾ遲厄ｼ啅FO縺ｨ鄒､繧後′蜷後§菴咲ｽｮ縺ｪ繧峨ョ繝輔か繝ｫ繝域婿蜷・
    float lengthSq = escapeDir.LengthSquare();
    if (lengthSq < 0.0001f)
    {
        escapeDir = VECTOR3(0, 0, 1);  // 繝・ヵ繧ｩ繝ｫ繝茨ｼ啝+譁ｹ蜷・
    }
    else
    {
        escapeDir = escapeDir / sqrtf(lengthSq);  // 豁｣隕丞喧
    }

    // 鄒､繧後・蠕後ｍ・・FO縺九ｉ隕九※鄒､繧後・蜷代％縺・・・峨↓遶九▽
    float pushDistance = 5.0f;
    m_targetPos = centroid + escapeDir * pushDistance;
}

void CDriving::Update()
{
    // 逶ｮ讓吩ｽ咲ｽｮ縺ｸ遘ｻ蜍・
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    direction.y = 0;
    const float distanceSq = direction.LengthSquare();

    constexpr float arrivalThresholdSq = 0.25f;
    // 逶ｮ讓吶↓蛻ｰ驕斐＠縺溘ｉ邨ゆｺ・
    if (distanceSq < arrivalThresholdSq)
    {
        m_isFinish = true;
        return;
    }

    normalize(direction);

    // 遘ｻ蜍墓婿蜷代↓蝗櫁ｻ｢
    float targetAngle = atan2f(direction.x, direction.z);
    m_pOwner->SetRotateY(targetAngle);

    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}

CRescue::CRescue(CAShepherdDog* dog)
{
    m_pOwner = dog;
    m_phase = Phase::APPROACH_SHEEP;
}

void CRescue::Enter()
{
    m_isFinish = false;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_RUN);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.5f);

    // 謨大勧繧ｭ繝･繝ｼ縺九ｉ蟇ｾ雎｡繧貞叙蠕・
    if (m_pOwner->GetRescueQueue().empty())
    {
        m_isFinish = true;
        return;
    }

    m_targetSheep = m_pOwner->GetRescueQueue().front();

    // 鄒､繧後・驥榊ｿ・ｒ險育ｮ・
    FlogInfo info = ObjectManager::FindGameObject<CFlog>()->CalcFlogInfo(m_pOwner->GetSheeps());
    m_centroid = info.centroid;

    // 繝輔ぉ繝ｼ繧ｺ1: 鄒翫↓霑代▼縺上→縺薙ｍ縺九ｉ髢句ｧ・
    m_phase = Phase::APPROACH_SHEEP;
}

void CRescue::Update()
{
    if (m_targetSheep == nullptr)
    {
        m_isFinish = true;
        return;
    }

    const VECTOR3 myPos = m_pOwner->GetTransform().position;
    const VECTOR3 sheepPos = m_targetSheep->GetTransform().position;
    constexpr float m_moveSpeed = 2.0f;

    switch (m_phase)
    {
    case Phase::APPROACH_SHEEP:
        {
            // 鄒翫∈縺ｮ譁ｹ蜷・
            const VECTOR3 toSheep = sheepPos - myPos;
            const float distance = toSheep.LengthSquare();
            constexpr float m_arrivalDistance = 1.0f; // 驥榊ｿ・芦驕泌愛螳夊ｷ晞屬

            // 鄒翫・霑代￥縺ｫ蛻ｰ驕斐＠縺溘ｉ繝輔ぉ繝ｼ繧ｺ2縺ｸ
            if (distance < Pow2(m_arrivalDistance))
            {
                m_phase = Phase::GUIDE_TO_CENTER;
                break;
            }

            // 鄒翫↓霑代▼縺・
            normalize(toSheep);
            // 遘ｻ蜍墓婿蜷代↓蝗櫁ｻ｢
            float targetAngle = atan2f(toSheep.x, toSheep.z);
            m_pOwner->SetRotateY(targetAngle);
            m_pOwner->AddPosition(toSheep * m_moveSpeed * SceneManager::DeltaTime());
        }
        break;

    case Phase::GUIDE_TO_CENTER:
        {
            // 鄒翫°繧蛾㍾蠢・∈縺ｮ譁ｹ蜷・
            VECTOR3 toCentroid = m_centroid - sheepPos;
            toCentroid.y = 0;
            const float distance = toCentroid.LengthSquare();

            // 驥榊ｿ・↓蛻ｰ驕斐＠縺溘ｉ螳御ｺ・
            constexpr float m_approachDistance = 2.0f; // 鄒翫↓霑代▼縺剰ｷ晞屬`
            if (distance < Pow2(m_approachDistance))
            {
                m_pOwner->PopRescueQueue(); // 繧ｭ繝･繝ｼ縺九ｉ蜑企勁
                m_isFinish = true;
                break;
            }

            // 鄒翫・閭悟ｾ後°繧蛾㍾蠢・婿蜷代∈繝励Ξ繝・す繝｣繝ｼ繧偵°縺代ｋ菴咲ｽｮ
            normalize(toCentroid);
            static constexpr float m_behindDistance = 2.0f; // 鄒翫・閭悟ｾ後↓遶九▽霍晞屬
            const VECTOR3 behindPos = sheepPos - toCentroid * m_behindDistance;

            // 縺昴・菴咲ｽｮ縺ｸ遘ｻ蜍・
            VECTOR3 toBehind = behindPos - myPos;
            toBehind.y = 0;
            normalize(toBehind);
            // 遘ｻ蜍墓婿蜷代↓蝗櫁ｻ｢
            float targetAngle = atan2f(toBehind.x, toBehind.z);
            m_pOwner->SetRotateY(targetAngle);
            m_pOwner->AddPosition(toBehind * m_moveSpeed * SceneManager::DeltaTime());
        }
        break;
    }
}


CDestroyShepherdDog::CDestroyShepherdDog(CAShepherdDog* dog, int score, float exp)
    : CDestroy(dog, score, exp)
{
    m_pDog = dog;
}

void CDestroyShepherdDog::Enter()
{
    // 諡・ｽ鍋ｾ翫ｒPANIC蛹・
    for (auto sheep : m_pDog->GetSheeps())
    {
        if (sheep != nullptr)
        {
            sheep->ChangeState(CBaseState::State::PANIC);
        }
    }

    // 蜈・・Destroy蜃ｦ逅・
    CDestroy::Enter();
}
