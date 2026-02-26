#include "Human.h"
#include "FunShape.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerHP.h"
#include "../../System/VisionSystem.h"
#include "../System/EnemyManager.h"
#include "../../Utils/BBox.h"
#include "../../Stage/Ground.h"
#include "../Component/Find.h"
#include "../Component/IdleHuman.h"
#include "../Component/Walk.h"

CHuman::CHuman(const VECTOR3& pos, const VECTOR2& areaSize)
    : m_AreaSize(areaSize)
{
    transform.position = pos;
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Human");
    m_pAnimator =  std::make_unique<Animator>();
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
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this,1.2f);
    m_components[CBaseState::State::FIND_PLAYER] = std::make_unique<CFind>(this);
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState = std::make_unique<CBaseState>(this);
    m_pState->Enter(CBaseState::State::IDLE);
    m_pState->SetNextState();
}

CHuman::~CHuman()
{
    m_components.clear();
}
void CHuman::Update()
{
    CEnemyBase::Update();

    // 削除フラグが立っている場合は、これ以上の処理を行わない
    if (m_pState != nullptr && m_pComponent == m_components[CBaseState::State::DESTROY].get())
    {
        return;
    }

    m_pAnimator->Update();
    CVisionSystem*vision = ObjectManager::FindGameObject<CVisionSystem>();
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

    // ステージオブジェクトとの衝突判定と押し戻し（最後に実行）
    ResolveStageCollisions();
}




void CHuman::Draw()
{
    m_pMesh->Render(m_pAnimator.get(), transform.matrix());
    //Debug関数
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


//Humanの範囲をLineで可視化
//範囲内なら水色、外なら緑になる
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
