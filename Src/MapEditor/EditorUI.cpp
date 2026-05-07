#include "EditorUI.h"
#include "Buttom.h"
#include "Controller.h"
#include "TRSObject/TRS.h"

namespace
{
    constexpr float WINDOW_Y = 10.0f;
    constexpr float OFFSET_FROM_RIGHT = 320.0f;
}

EditorUI::EditorUI()
{
    m_pButton = ObjectManager::FindGameObject<Button>();
    m_pTRS = ObjectManager::FindGameObject<TRS>();
    m_pController = ObjectManager::FindGameObject<Controller>();
}

void EditorUI::Draw()
{
    DrawEditorToolsWindow();
    DrawSettingWindow();
    DrawHierarchyWindow();
    DrawTransformWindow();
    DrawMoveAmountWindow();
}

void EditorUI::DrawEditorToolsWindow() const
{
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x - OFFSET_FROM_RIGHT, WINDOW_Y),
        ImGuiCond_FirstUseEver);
    ImGui::Begin("Editor Tools");
    m_pButton->DrawEditorToolsPanel();
    ImGui::End();
}

void EditorUI::DrawSettingWindow() const
{
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x - OFFSET_FROM_RIGHT, WINDOW_Y),
        ImGuiCond_FirstUseEver);
    ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    m_pButton->DrawSettingPanel();
    ImGui::Separator();
    m_pController->DrawSettingPanel();
    ImGui::Separator();
    m_pController->DrawCollisionBox();
    ImGui::End();
}

void EditorUI::DrawHierarchyWindow() const
{
    ImGui::SetNextWindowPos(ImVec2(10, 180), ImGuiCond_Once);
    ImGui::SetNextWindowSizeConstraints(ImVec2(120, 0), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    m_pButton->DrawHierarchyPanel();
    ImGui::End();
}

void EditorUI::DrawTransformWindow() const
{
    if (!m_pController->HasSelectedObject()) return;
    constexpr float WINDOW_X = 10.0f;
    constexpr float WINDOW_W = 300.0f;
    constexpr float WINDOW_H = 160.0f;
    ImGui::SetNextWindowPos(ImVec2(WINDOW_X, WINDOW_Y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(WINDOW_W, WINDOW_H), ImGuiCond_Once);
    ImGui::Begin("Transform");
    m_pController->DrawTransformPanel();
    ImGui::End();
}

void EditorUI::DrawMoveAmountWindow() const
{
    ImGui::SetNextWindowPos(ImVec2(350, WINDOW_Y), ImGuiCond_Once);
    ImGui::Begin("MoveAmount", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    m_pTRS->DrawPanel();
    ImGui::End();
}
