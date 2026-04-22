#include "Human.h"
#include "FunShape.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerHP.h"
#include "../../System/VisionSystem.h"
#include "../System/EnemyManager.h"
#include "../../Utils/BBox.h"
#include "../../Stage/Ground.h"
#include "../Component/ComponentFwd.h"

CHuman::CHuman(const VECTOR3& pos, const VECTOR2& areaSize)
    : m_AreaSize(areaSize), m_pFunShape(nullptr)
{
    transform.position = pos;
    auto m = ObjectManager::FindGameObject<CEnemyManager>();
    if (m == nullptr)
        assert(false);
    m_pMesh = m->MeshList(Animal::Name::HUMAN);
    if (m_pMesh == nullptr)
        assert(false);
    m_pAnimator = std::make_unique<Animator>();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);
    m_pAnimator->SetPlaySpeed(1.0f);
    angle = 0.0f;

    InitStates();

    m_pFunShape = Instantiate<CFunShape>();
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pBBox = CreateBBox();
    m_pFunShape->SetParent(this);
    m_isHuman = true;
}

void CHuman::InitStates()
{
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdleHuman>(this);
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this, 1.2f);
    m_components[CBaseState::State::FIND_PLAYER] = std::make_unique<CFind>(this);
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState = std::make_unique<CBaseState>(this);
    m_pState->Enter(CBaseState::State::IDLE);
    m_pState->SetNextState();
}

CHuman::~CHuman()
{
    m_components.clear();
    if (m_pFunShape != nullptr)
    {
        ObjectManager::DeleteGameObject(m_pFunShape);
        m_pFunShape = nullptr;
    }
}

void CHuman::Update()
{
    CEnemyBase::Update();

    // 蜑企勁繝輔Λ繧ｰ縺檎ｫ九▲縺ｦ縺・ｋ蝣ｴ蜷医・縲√％繧御ｻ･荳翫・蜃ｦ逅・ｒ陦後ｏ縺ｪ縺・
    if (m_pState != nullptr && m_pComponent == m_components[CBaseState::State::DESTROY].get())
    {
        return;
    }

    m_pAnimator->Update();
    CVisionSystem* vision = ObjectManager::FindGameObject<CVisionSystem>();
    m_inSight = vision->SectorCircleCollision(ToVec2XZ(transform.position), transform.rotation.y)
        && ObjectManager::FindGameObject<CPlayer>()->GetIsSuckUp();


    if (m_inSight)
    {
        ChangeState(CBaseState::State::FIND_PLAYER);
    }
    else
    {
        ObjectManager::FindGameObject<CPlayerHP>()->ResetFlag();
    }
    AtkArea();
    ResolveOBBCollisions();
    UpdateBBox();

    // 繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ陦晉ｪ∝愛螳壹→謚ｼ縺玲綾縺暦ｼ域怙蠕後↓螳溯｡鯉ｼ・
    ResolveStageCollisions();
}


void CHuman::Draw()
{
    m_pMesh->Render(m_pAnimator.get(), transform.matrix());
    //Debug髢｢謨ｰ
    //DrawDirectionLine();
    //FanShape();
}

void CHuman::AtkArea() const
{
    if (m_pFunShape)
    {
        m_pFunShape->PosSet(transform.position, angle + transform.rotation.y);
    }
}


//Human縺ｮ遽・峇繧鱈ine縺ｧ蜿ｯ隕門喧
//遽・峇蜀・↑繧画ｰｴ濶ｲ縲∝､悶↑繧臥ｷ代↓縺ｪ繧・
//Debug
// void CHuman::DrawDirectionLine()
// {
//     CSprite spr;
//     const MATRIX4X4 mat = XMMatrixRotationY(angle + transform.rotation.y);
//
//     const VECTOR3 startPos = transform.position;
//
//     const VECTOR3 endPos = startPos + VECTOR3(0, 0, LINE_LENGTH) * mat;
//     
//    // spr.DrawLine3D(startPos, endPos, RGB(0, 255, m_dwColor));
// }

//Dugub//
// void CHuman::FanShape()
// {
//     CSprite spr;
//     
//     float angle = -(20.0f * DegToRad);
//     
//     for (int i = 0; i < 3; i++)
//     {
//         if (i == 2) angle = std::abs(angle);
//
//         MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y + angle);
//         
//         VECTOR3 startPos = transform.position;
//         VECTOR3 endPos = startPos + VECTOR3(0, 0, 7.0f) * mat;
//
//         spr.DrawLine3D(startPos, endPos, RGB(255, 0, 0));
//     }
// }
