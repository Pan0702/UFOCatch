#include "Controller.h"

#include "MouseRay.h"
#include "UndoManager.h"
#include "StageData.h"
#include "TRSObject/TRS.h"

namespace
{
    // Transform 繧ｦ繧｣繝ｳ繝峨え縺ｮ蛻晄悄菴咲ｽｮ縺ｨ繧ｵ繧､繧ｺ
    constexpr float kTransformWindowX    = 10.0f;
    constexpr float kTransformWindowY    = 10.0f;
    constexpr float kTransformWindowW    = 300.0f;
    constexpr float kTransformWindowH    = 160.0f;

    // DragFloat3 縺ｮ1繝輔Ξ繝ｼ繝縺ゅ◆繧翫・螟牙喧驥・
    constexpr float kDragSpeed = 0.1f;
}

Controller::Controller()
{
    m_pCamera = ObjectManager::FindGameObject<Camera>();
    m_pTrs = ObjectManager::FindGameObject<TRS>();
    m_pStageData = ObjectManager::FindGameObject<StageData>();
    m_pInput = GameDevice()->m_pDI;
    m_pUndoManager = std::make_unique<UndoManager>();
    m_pRandomPlacer = std::make_unique<RandomPlacer>();
}

void Controller::SetCatchFlag(bool f)
{
    m_isCatch = f;
}


void Controller::Update()
{
    const ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureMouse)
    {
        // 蜿ｳ繧ｯ繝ｪ繝・け荳ｭ縺ｯ繧ｫ繝｡繝ｩ謫堺ｽ懊ｒ蜆ｪ蜈医＠縲ゝRS 謫堺ｽ懊・辟｡蜉ｹ
        if (m_pInput->CheckMouse(KD_DAT, DIM_RBUTTON))
        {
            CameraControl();
        }
        else
        {
            if (m_isCatch)
            {
                // 繧ｪ繝悶ず繧ｧ繧ｯ繝磯∈謚樔ｸｭ縺ｮ縺ｿ繧ｭ繝ｼ縺ｫ繧医ｋ繝｢繝ｼ繝牙・譖ｿ繧貞女縺台ｻ倥￠繧・
                TRSControl();
            }
            else
            {
                // 譛ｪ驕ｸ謚樊凾縺ｯ繧ｮ繧ｺ繝｢繧帝撼陦ｨ遉ｺ縺ｫ縺吶ｋ
                m_pTrs->SetState(TRS::State::kNone);
            }
        }

        if (m_pInput->GetMouseWheel() != 0)
        {
            Camera::Zoom();
        }

        // 繧ｯ繝ｪ繝・け縺励◆迸ｬ髢・
        if (m_pInput->CheckMouse(KD_TRG, DIM_LBUTTON))
        {
            HandleLeftClick();
        }

        // 蟾ｦ繧ｯ繝ｪ繝・け髮｢縺玲凾縺ｫ繝峨Λ繝・げ繧定ｧ｣髯､
        if (m_isCatch && m_pInput->CheckMouse(KD_UTRG, DIM_LBUTTON))
        {
            m_pTrs->SetDraggingAxis(Axis::None);
        }
    }

    if (!io.WantCaptureKeyboard)
    {
        if (m_pInput->CheckKey(KD_TRG, DIK_F))
        {
            Camera::Focus();
        }

        if (m_isCatch)
        {
            // BackSpace / Delete 縺ｧ繧ｪ繝悶ず繧ｧ繧ｯ繝亥炎髯､
            bool is_delete = m_pInput->CheckKey(KD_TRG, DIK_BACK) || m_pInput->CheckKey(KD_TRG, DIK_DELETE);
            if (is_delete)
            {
                m_pTrs->SetState(TRS::State::kNone);
                m_pUndoManager->DeleteObjectPush();
                m_pStageData->DeleteModel();
                m_isCatch = false;
            }
        }

        if (m_pInput->CheckKey(KD_TRG, DIK_C) && m_pInput->CheckKey(KD_DAT, DIK_LCONTROL))
        {
            m_copyObjectIndex = m_pStageData->GetSelectIndex();
        }
        if (m_pInput->CheckKey(KD_TRG, DIK_V) && m_pInput->CheckKey(KD_DAT, DIK_LCONTROL))
        {
            m_pStageData->CopyModel(m_copyObjectIndex);
        }

        HandleUndoRedo();
    }

}

// 蟾ｦ繧ｯ繝ｪ繝・け譎ゅ↓TRS繧ｮ繧ｺ繝｢縺ｾ縺溘・繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝ｬ繧､蛻､螳壹ｒ陦後≧
void Controller::HandleLeftClick()
{
    const Ray ray = MouseRay::Create();

    // 繧ｮ繧ｺ繝｢縺ｸ縺ｮ繧ｯ繝ｪ繝・け繧貞━蜈亥愛螳壹ょｽ薙◆縺｣縺溷ｴ蜷医・繧ｪ繝悶ず繧ｧ繧ｯ繝磯∈謚槫愛螳壹ｒ陦後ｏ縺ｪ縺・
    const Axis a = m_pTrs->RayHitTest(ray);
    if (a != Axis::None)
    {
        if (m_isRandomPlacer)
        {
            m_pUndoManager->Push(m_pRandomPlacer->GetTransform());
        }else
        {
            // 繝峨Λ繝・げ髢句ｧ句燕縺ｫ迴ｾ迥ｶ諷九ｒ菫晏ｭ・
            m_pUndoManager->Push();  
        }
  
        m_pTrs->SetDraggingAxis(a);
        return;
    }

    // 繧ｮ繧ｺ繝｢縺ｫ蠖薙◆繧峨↑縺九▲縺溷ｴ蜷医・繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医・驕ｸ謚槫愛螳・
    MeshCollider::CollInfo hit;
    const int index = m_pStageData->RayHitTest(ray, &hit);
    if (index >= 0)
    {
        m_isCatch = true;
        m_pStageData->SetModel(index);
    }
    else
    {
        m_isCatch = false;
    }
}

// Ctrl+Z/Ctrl+Y縺ｧUndo/Redo繧貞ｮ溯｡後☆繧・
void Controller::HandleUndoRedo() const
{
    if (m_pInput->CheckKey(KD_DAT, DIK_LCONTROL))
    {
        if (m_pInput->CheckKey(KD_TRG, DIK_Z)) m_pUndoManager->Undo();
        if (m_pInput->CheckKey(KD_TRG, DIK_Y)) m_pUndoManager->Redo();
    }
}

void Controller::Random()
{
    if (m_pRandomPlacer == nullptr) return;  
    if (m_isRandomPlacer)
    {
        m_pRandomPlacer->SetDrawFlag(true);
        m_pTrs->SetOverrideTarget(m_pRandomPlacer->GetTransform());
        m_pTrs->SetState(TRS::State::kTranslation);
        m_isCatch = true;  // 繧ｮ繧ｺ繝｢陦ｨ遉ｺON
    }else
    {
        m_pRandomPlacer->SetDrawFlag(false);
        m_pTrs->SetOverrideTarget(nullptr);
        m_pTrs->SetState(TRS::State::kNone);
        m_isCatch = false;
    }
}

// W/E/R/Q繧ｭ繝ｼ縺ｧ繧｢繧ｯ繝・ぅ繝悶↑繧ｮ繧ｺ繝｢繝｢繝ｼ繝峨ｒ蛻・ｊ譖ｿ縺医ｋ
void Controller::TRSControl() const
{
    if (m_pInput->CheckKey(KD_TRG, DIK_W))
    {
        m_pTrs->SetState(TRS::State::kTranslation);
    }

    if (m_pInput->CheckKey(KD_TRG, DIK_E))
    {
        m_pTrs->SetState(TRS::State::kRotation);
    }

    if (m_pInput->CheckKey(KD_TRG, DIK_R))
    {
        m_pTrs->SetState(TRS::State::kScaling);
    }

    if (m_pInput->CheckKey(KD_TRG, DIK_Q))
    {
        m_pTrs->SetState(TRS::State::kNone);
    }
}

// 蜿ｳ繧ｯ繝ｪ繝・け荳ｭ縺ｮ繝槭え繧ｹ遘ｻ蜍輔・繧ｭ繝ｼ蜈･蜉帙〒繧ｫ繝｡繝ｩ繧呈桃菴懊☆繧・
void Controller::CameraControl() const
{
    //蝗櫁ｻ｢
    if (m_pInput->IsMouseMove())
    {
        Camera::Rotate();
    }
    //遘ｻ蜍・
    if (m_pInput->IsMoveInput())
    {
        Camera::Move();
    }
}

void Controller::Draw()
{
    ImGui::Begin("Setting");
    if (ImGui::Checkbox("Random Placer", &m_isRandomPlacer))
    {
        Random();
    }

    ImGui::End();
    if (m_isRandomPlacer)
    {
        m_pRandomPlacer->Draw(); 
    }
    Transform* t = m_pStageData ? m_pStageData->GetSelectedTransform() : nullptr;
    if (not t) return;
    if (not m_isCatch)return;
    ImGui::SetNextWindowPos(ImVec2(kTransformWindowX, kTransformWindowY), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(kTransformWindowW, kTransformWindowH), ImGuiCond_Once);
    ImGui::Begin("Transform");

    ImGui::Separator();
    ImGui::Text("Position: %.2f, %.2f, %.2f", t->position.x,
                t->position.y, t->position.z);
    if (ImGui::DragFloat3("Position", &t->position.x, kDragSpeed))
    {
    }
    if (ImGui::IsItemActivated())
    {
        m_pUndoManager->Push();
    }
    ImGui::Separator();
    VECTOR3& tmp_r = t->rotation;
    ImGui::Text("Rotation: %.2f, %.2f, %.2f", tmp_r.x,
                tmp_r.y, tmp_r.z);
    if (ImGui::DragFloat3("Rotation", &tmp_r.x, kDragSpeed))
    {
    }
    if (ImGui::IsItemActivated())
    {
        m_pUndoManager->Push();
    }
    ImGui::Separator();
    ImGui::Text("Scale:    %.2f, %.2f, %.2f", t->scale.x,
                t->scale.y, t->scale.z);
    if (ImGui::DragFloat3("Scale", &t->scale.x, kDragSpeed))
    {
    }
    if (ImGui::IsItemActivated())
    {
        m_pUndoManager->Push();
    }
    ImGui::Separator();
    ImGui::End();

}

