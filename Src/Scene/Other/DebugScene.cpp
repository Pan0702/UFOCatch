#include "DebugScene.h"

#include "../../Scene/Other/DebugCamera.h"
#include "../../System/GameInstance.h"
#include "../../Enemies/System/EnemyManager.h"
#include "../../Enemies/System/Flock.h"
#include "../../Enemies/AnimalSheep/Sheep.h"
#include "../../Stage/StageFactor.h"
#include "../../Utils/Sprite3D.h"
#include "../../../Libs/Imgui/imgui.h"
#include "../../Player/PCamera.h"
#include "../../System/VisionSystem.h"

namespace
{
    const char* SheepStateName(CBaseState::State s)
    {
        switch (s)
        {
        case CBaseState::State::IDLE: return "IDLE";
        case CBaseState::State::WALK: return "WALK";
        case CBaseState::State::SUCTION: return "SUCTION";
        case CBaseState::State::DESTROY: return "DESTROY";
        case CBaseState::State::HERDED: return "HERDED";
        case CBaseState::State::PANIC: return "PANIC";
        default: return "OTHER";
        }
    }

    const char* DogStateName(CBaseState::State s)
    {
        switch (s)
        {
        case CBaseState::State::IDLE: return "IDLE";
        case CBaseState::State::WALK: return "WALK";
        case CBaseState::State::COLLECTING: return "COLLECTING";
        case CBaseState::State::DRIVING: return "DRIVING";
        case CBaseState::State::RESCUE: return "RESCUE";
        case CBaseState::State::DESTROY: return "DESTROY";
        default: return "OTHER";
        }
    }
}

CDebugScene::CDebugScene()
{
    Instantiate<CStageFactor>();
    SingleInstantiate<CEnemyManager>();
    Instantiate<CPlayerCamera>();
    Instantiate<CVisionSystem>();
    Instantiate<CPlayer>(50);
    Instantiate<CDebugCamera>();
    auto* flock = Instantiate<CFlock>(VECTOR3(0.0f, 0, 0), 2.0f, 1);
    CGameInstance::Get()->Init(1000);
}

CDebugScene::~CDebugScene()
{
    SAFE_DELETE(m_pDebugUI);
}

void CDebugScene::ChangeResultScene()
{
    SceneManager::ChangeScene("ResultScene");
}

void CDebugScene::Update()
{
    UpdateImguiPanel();
}

void CDebugScene::Draw()
{
    DrawFlockCircles();
    DrawMaxFlockCircles();
    DrawHalfFlockCircles();
}

void CDebugScene::DrawHalfFlockCircles() const
{
    constexpr int SEGMENTS = 32;
    constexpr DWORD COLOR = RGB(0, 255, 255);

    CSprite spr;
    for (CFlock* flock : ObjectManager::FindGameObjects<CFlock>())
    {
        const VECTOR3 c = flock->GetFlockCenter();
        const float r = flock->GetMoveRadius() * 0.7;

        VECTOR3 prev(c.x + r, c.y, c.z);
        for (int i = 1; i <= SEGMENTS; ++i)
        {
            const float t = static_cast<float>(i) / SEGMENTS * XM_2PI;
            VECTOR3 next(c.x + cosf(t) * r, c.y, c.z + sinf(t) * r);
            spr.DrawLine3D(prev, next, COLOR);
            prev = next;
        }
    }
}

void CDebugScene::DrawFlockCircles() const
{
    constexpr int SEGMENTS = 32;
    constexpr DWORD COLOR = RGB(0, 255, 255);

    CSprite spr;
    for (CFlock* flock : ObjectManager::FindGameObjects<CFlock>())
    {
        const VECTOR3 c = flock->GetFlockCenter();
        const float r = flock->GetFlockRadius();

        VECTOR3 prev(c.x + r, c.y, c.z);
        for (int i = 1; i <= SEGMENTS; ++i)
        {
            const float t = static_cast<float>(i) / SEGMENTS * XM_2PI;
            VECTOR3 next(c.x + cosf(t) * r, c.y, c.z + sinf(t) * r);
            spr.DrawLine3D(prev, next, COLOR);
            prev = next;
        }
    }
}

void CDebugScene::DrawMaxFlockCircles() const
{
    constexpr int SEGMENTS = 32;
    constexpr DWORD COLOR = RGB(0, 255, 255);

    CSprite spr;
    for (CFlock* flock : ObjectManager::FindGameObjects<CFlock>())
    {
        const VECTOR3 c = flock->GetFlockCenter();
        const float r = flock->GetMoveRadius();

        VECTOR3 prev(c.x + r, c.y, c.z);
        for (int i = 1; i <= SEGMENTS; ++i)
        {
            const float t = static_cast<float>(i) / SEGMENTS * XM_2PI;
            VECTOR3 next(c.x + cosf(t) * r, c.y, c.z + sinf(t) * r);
            spr.DrawLine3D(prev, next, COLOR);
            prev = next;
        }
    }
}

void CDebugScene::UpdateImguiPanel()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(360, 500), ImGuiCond_Once);

    if (!ImGui::Begin("Debug Panel"))
    {
        ImGui::End();
        return;
    }

    // ---- 犬の状態 ----
    if (ImGui::CollapsingHeader("Dog", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (m_pDog != nullptr)
        {
            ImGui::Text("State: %s", DogStateName(m_pDog->GetCurrentState()));
            ImGui::Text("Sheep count: %zu", m_pDog->GetSheeps().size());
            ImGui::Text("Rescue queue: %zu", m_pDog->GetRescueQueue().size());
            if (ImGui::Button("Destroy Dog"))
            {
                m_pDog->ChangeState(CBaseState::State::DESTROY);
                m_pDog = nullptr;
            }
        }
        else
        {
            ImGui::Text("(no dog)");
        }
    }

    // ---- Flock 一覧 + 追加フォーム ----
    if (ImGui::CollapsingHeader("Flocks", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto flocks = ObjectManager::FindGameObjects<CFlock>();
        int flockIdx = 0;
        for (CFlock* flock : flocks)
        {
            const VECTOR3 c = flock->GetFlockCenter();
            ImGui::Text("Flock[%d]  center=(%.1f, %.1f)  r=%.1f  sheep=%zu",
                        flockIdx, c.x, c.z, flock->GetFlockRadius(), flock->GetAllSheeps().size());
            ++flockIdx;
        }

        ImGui::Separator();
        ImGui::Text("Add new Flock:");
        ImGui::InputFloat("Center X", &m_newFlockCenterX);
        ImGui::InputFloat("Center Z", &m_newFlockCenterZ);
        ImGui::InputFloat("Radius", &m_newFlockRadius);
        ImGui::InputInt("Sheep count", &m_newFlockSheepCount);
        if (ImGui::Button("Spawn Flock"))
        {
            Instantiate<CFlock>(
                VECTOR3(m_newFlockCenterX, 0.0f, m_newFlockCenterZ),
                m_newFlockRadius,
                m_newFlockSheepCount);
        }
    }

    // ---- 羊の状態一覧 ----
    if (ImGui::CollapsingHeader("Sheeps", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto sheeps = ObjectManager::FindGameObjects<CSheep>();
        ImGui::Text("Total: %zu", sheeps.size());
        ImGui::Separator();

        int sheepIdx = 0;
        for (CSheep* sheep : sheeps)
        {
            CFlock* flock = sheep->GetFlock();
            const VECTOR3 p = sheep->GetTransform().position;

            ImGui::PushID(sheepIdx);
            ImGui::Text("[%d] %s flock=%p pos=(%.1f,%.1f)",
                        sheepIdx,
                        SheepStateName(sheep->GetCurrentState()),
                        static_cast<void*>(flock),
                        p.x, p.z);
            ImGui::SameLine();
            if (ImGui::Button("Kill"))
            {
                sheep->ChangeState(CBaseState::State::DESTROY);
            }
            ImGui::PopID();
            ++sheepIdx;
        }
    }

    ImGui::End();
}
