#include "Controller.h"
#include "MouseRay.h"
#include "UndoManager.h"
#include "StageData.h"
#include "TRSObject/TRS.h"

namespace
{
    // Transform ウィンドウの初期位置とサイズ
    constexpr float kTransformWindowX = 10.0f;
    constexpr float kTransformWindowY = 10.0f;
    constexpr float kTransformWindowW = 300.0f;
    constexpr float kTransformWindowH = 160.0f;

    // DragFloat3 の1フレームあたりの変化量
    constexpr float kDragSpeed = 0.1f;
}

Controller::Controller()
{
    m_pTrs = ObjectManager::FindGameObject<TRS>();
    m_pStageData = ObjectManager::FindGameObject<StageData>();
    m_pInput = GameDevice()->m_pDI;
    m_pUndoManager = std::make_unique<CUndoManager>();
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
        const bool is_alt = m_pInput->CheckKey(KD_DAT, DIK_LMENU)
            || m_pInput->CheckKey(KD_DAT, DIK_RMENU);

        // Alt 系のUE風カメラ操作を最優先する
        if (is_alt && (m_pInput->CheckMouse(KD_DAT, DIM_LBUTTON)
            || m_pInput->CheckMouse(KD_DAT, DIM_MBUTTON)
            || m_pInput->CheckMouse(KD_DAT, DIM_RBUTTON)))
        {
            CameraControl();
        }
        // 右クリック中はカメラ操作を優先し、TRS 操作は無効
        else if (m_pInput->CheckMouse(KD_DAT, DIM_RBUTTON))
        {
            CameraControl();
        }
        else
        {
            if (m_isCatch)
            {
                // オブジェクト選択中のみキーによるモード切替を受け付ける
                TRSControl();
            }
            else
            {
                // 未選択時はギズモを非表示にする
                m_pTrs->SetState(TRS::State::kNone);
            }
        }

        if (m_pInput->GetMouseWheel() != 0)
        {
            Camera::Zoom();
        }

        // BackSpace / Delete でオブジェクト削除
        if (!is_alt && m_pInput->CheckMouse(KD_TRG, DIM_LBUTTON))
        {
            HandleLeftClick();
        }

        // 左クリック離し時にドラッグを解除
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
            // BackSpace / Delete でオブジェクト削除
            bool is_delete = m_pInput->CheckKey(KD_TRG, DIK_BACK) || m_pInput->CheckKey(KD_TRG, DIK_DELETE);
            if (is_delete)
            {
                m_pTrs->SetState(TRS::State::kNone);
                m_pUndoManager->Push(std::make_unique<CDeleteCommand>());
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

// 左クリック時にTRSギズモまたはステージオブジェクトへのレイ判定を行う
void Controller::HandleLeftClick()
{
    const Ray ray = MouseRay::Create();

    // オブジェクト選択中のみTRSギズモへのレイ判定を行う
    const Axis a = m_pTrs->RayHitTest(ray);
    if (a != Axis::None)
    {
        if (m_isRandomPlacer)
        {
            m_pUndoManager->Push(std::make_unique<CRawTransformCommand>(m_pRandomPlacer->GetTransform()));
        }
        else
        {
            // ドラッグ開始前に現状を保存
            m_pUndoManager->Push(std::make_unique<CTransformCommand>());
        }

        m_pTrs->SetDraggingAxis(a);
        return;
    }

    // ギズモに当たらなかった場合はステージオブジェクトの選択判定
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

// Ctrl+Z/Ctrl+YでUndo/Redoを実行する
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
        m_isCatch = true; // ギズモ表示ON
    }
    else
    {
        m_pRandomPlacer->SetDrawFlag(false);
        m_pTrs->SetOverrideTarget(nullptr);
        m_pTrs->SetState(TRS::State::kNone);
        m_isCatch = false;
    }
}

// W/E/R/Qキーでアクティブなギズモモードを切り替える
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

// 右クリック中のマウス移動やキー入力でカメラを操作する
void Controller::CameraControl() const
{
    const bool is_alt = m_pInput->CheckKey(KD_DAT, DIK_LMENU)
        || m_pInput->CheckKey(KD_DAT, DIK_RMENU);

    if (is_alt)
    {
        if (m_pInput->CheckMouse(KD_DAT, DIM_LBUTTON))
        {
            Camera::Orbit();
            return;
        }
        if (m_pInput->CheckMouse(KD_DAT, DIM_MBUTTON))
        {
            Camera::Pan();
            return;
        }
        if (m_pInput->CheckMouse(KD_DAT, DIM_RBUTTON))
        {
            Camera::Dolly();
            return;
        }
    }

    //マウスによる回転
    if (m_pInput->IsMouseMove())
    {
        Camera::Rotate();
    }
    //キーボードによる移動
    if (m_pInput->IsMoveInput())
    {
        Camera::Move();
    }
}

void Controller::Draw()
{
    if (m_isRandomPlacer)
        m_pRandomPlacer->Draw();
}

void Controller::DrawSettingPanel()
{
    if (ImGui::Checkbox("Random Placer", &m_isRandomPlacer))
        Random();
    if (m_isRandomPlacer)
        m_pRandomPlacer->DrawPanel();
}

void Controller::DrawCollisionBox()
{
    static StageColl c = m_pStageData->GetColl();

    if (ImGui::Checkbox("UseOBB", &c.useOBB))
    {
        m_pStageData->SetColl(c);
    }
    if (ImGui::Checkbox("UseHitGround", &c.useHitGround))
    {
        m_pStageData->SetColl(c);
    }
}

void Controller::DrawTransformPanel()
{
    Transform* t = m_pStageData ? m_pStageData->GetSelectedTransform() : nullptr;
    if (!t || !m_isCatch) return;

    ImGui::Separator();
    ImGui::Text("Position: %.2f, %.2f, %.2f", t->position.x,
                t->position.y, t->position.z);
    if (ImGui::DragFloat3("Position", &t->position.x, kDragSpeed))
    {
    }
    if (ImGui::IsItemActivated()) m_pUndoManager->Push(std::make_unique<CTransformCommand>());

    ImGui::Separator();
    VECTOR3& tmp_r = t->rotation;
    ImGui::Text("Rotation: %.2f, %.2f, %.2f", tmp_r.x,
                tmp_r.y, tmp_r.z);
    if (ImGui::DragFloat3("Rotation", &tmp_r.x, kDragSpeed))
    {
    }
    if (ImGui::IsItemActivated()) m_pUndoManager->Push(std::make_unique<CTransformCommand>());

    ImGui::Separator();
    ImGui::Text("Scale:    %.2f, %.2f, %.2f", t->scale.x,
                t->scale.y, t->scale.z);
    if (ImGui::DragFloat3("Scale", &t->scale.x, kDragSpeed))
    {
    }
    if (ImGui::IsItemActivated()) m_pUndoManager->Push(std::make_unique<CTransformCommand>());

    ImGui::Separator();
}

bool Controller::HasSelectedObject() const
{
    return m_isCatch && m_pStageData && m_pStageData->GetSelectedTransform() != nullptr;
}
