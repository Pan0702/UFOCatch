#include "Controller.h"

#include "MouseRay.h"
#include "UndoManager.h"
#include "StageData.h"
#include "TRSObject/TRS.h"

namespace
{
    // Transform ウィンドウの初期位置とサイズ
    constexpr float kTransformWindowX    = 10.0f;
    constexpr float kTransformWindowY    = 10.0f;
    constexpr float kTransformWindowW    = 300.0f;
    constexpr float kTransformWindowH    = 160.0f;

    // DragFloat3 の1フレームあたりの変化量
    constexpr float kDragSpeed = 0.1f;
}

Controller::Controller()
{
    camera_ = ObjectManager::FindGameObject<Camera>();
    trs_ = ObjectManager::FindGameObject<TRS>();
    stage_data_ = ObjectManager::FindGameObject<StageData>();
    input_ = GameDevice()->m_pDI;
    undo_manager_ = new UndoManager();
    random_placer_ = new RandomPlacer();//std::make_unique<RandomPlacer>();
}

void Controller::SetCatchFlag(bool f)
{
    is_catch_ = f;
}


void Controller::Update()
{
    const ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureMouse)
    {
        // 右クリック中はカメラ操作を優先し、TRS 操作は無効
        if (input_->CheckMouse(KD_DAT, DIM_RBUTTON))
        {
            CameraControl();
        }
        else
        {
            if (is_catch_)
            {
                // オブジェクト選択中のみキーによるモード切替を受け付ける
                TRSControl();
            }
            else
            {
                // 未選択時はギズモを非表示にする
                trs_->SetState(TRS::State::kNone);
            }
        }

        if (input_->GetMouseWheel() != 0)
        {
            Camera::Zoom();
        }

        // クリックした瞬間
        if (input_->CheckMouse(KD_TRG, DIM_LBUTTON))
        {
            HandleLeftClick();
        }

        // 左クリック離し時にドラッグを解除
        if (is_catch_ && input_->CheckMouse(KD_UTRG, DIM_LBUTTON))
        {
            trs_->SetDraggingAxis(Axis::None);
        }
    }

    if (!io.WantCaptureKeyboard)
    {
        if (input_->CheckKey(KD_TRG, DIK_F))
        {
            Camera::Focus();
        }

        if (is_catch_)
        {
            // BackSpace / Delete でオブジェクト削除
            bool is_delete = input_->CheckKey(KD_TRG, DIK_BACK) || input_->CheckKey(KD_TRG, DIK_DELETE);
            if (is_delete)
            {
                trs_->SetState(TRS::State::kNone);
                undo_manager_->DeleteObjectPush();
                stage_data_->DeleteModel();
                is_catch_ = false;
            }
        }

        if (input_->CheckKey(KD_TRG, DIK_C) && input_->CheckKey(KD_DAT, DIK_LCONTROL))
        {
            copy_object_index_ = stage_data_->GetSelectIndex();
        }
        if (input_->CheckKey(KD_TRG, DIK_V) && input_->CheckKey(KD_DAT, DIK_LCONTROL))
        {
            stage_data_->CopyModel(copy_object_index_);
        }

        HandleUndoRedo();
    }

}

// 左クリック時にTRSギズモまたはステージオブジェクトへのレイ判定を行う
void Controller::HandleLeftClick()
{
    const Ray ray = MouseRay::Create();

    // ギズモへのクリックを優先判定。当たった場合はオブジェクト選択判定を行わない
    const Axis a = trs_->RayHitTest(ray);
    if (a != Axis::None)
    {
        if (is_random_placer_)
        {
            undo_manager_->Push(random_placer_->GetTransform());
        }else
        {
            // ドラッグ開始前に現状態を保存
            undo_manager_->Push();  
        }
  
        trs_->SetDraggingAxis(a);
        return;
    }

    // ギズモに当たらなかった場合はステージオブジェクトの選択判定
    MeshCollider::CollInfo hit;
    const int index = stage_data_->RayHitTest(ray, &hit);
    if (index >= 0)
    {
        is_catch_ = true;
        stage_data_->SetModel(index);
    }
    else
    {
        is_catch_ = false;
    }
}

// Ctrl+Z/Ctrl+YでUndo/Redoを実行する
void Controller::HandleUndoRedo() const
{
    if (input_->CheckKey(KD_DAT, DIK_LCONTROL))
    {
        if (input_->CheckKey(KD_TRG, DIK_Z)) undo_manager_->Undo();
        if (input_->CheckKey(KD_TRG, DIK_Y)) undo_manager_->Redo();
    }
}

void Controller::Random()
{
    if (random_placer_ == nullptr) return;  
    if (is_random_placer_)
    {
        random_placer_->SetDrawFlag(true);
        trs_->SetOverrideTarget(random_placer_->GetTransform());
        trs_->SetState(TRS::State::kTranslation);
        is_catch_ = true;  // ギズモ表示ON
    }else
    {
        random_placer_->SetDrawFlag(false);
        trs_->SetOverrideTarget(nullptr);
        trs_->SetState(TRS::State::kNone);
        is_catch_ = false;
    }
}

// W/E/R/Qキーでアクティブなギズモモードを切り替える
void Controller::TRSControl() const
{
    if (input_->CheckKey(KD_TRG, DIK_W))
    {
        trs_->SetState(TRS::State::kTranslation);
    }

    if (input_->CheckKey(KD_TRG, DIK_E))
    {
        trs_->SetState(TRS::State::kRotation);
    }

    if (input_->CheckKey(KD_TRG, DIK_R))
    {
        trs_->SetState(TRS::State::kScaling);
    }

    if (input_->CheckKey(KD_TRG, DIK_Q))
    {
        trs_->SetState(TRS::State::kNone);
    }
}

// 右クリック中のマウス移動・キー入力でカメラを操作する
void Controller::CameraControl() const
{
    //回転
    if (input_->IsMouseMove())
    {
        Camera::Rotate();
    }
    //移動
    if (input_->IsMoveInput())
    {
        Camera::Move();
    }
}

void Controller::Draw()
{
    ImGui::Begin("Setting");
    if (ImGui::Checkbox("Random Placer", &is_random_placer_))
    {
        Random();
    }

    ImGui::End();
    if (is_random_placer_)
    {
        random_placer_->Draw(); 
    }
    Transform* t = stage_data_ ? stage_data_->GetSelectedTransform() : nullptr;
    if (not t) return;
    if (not is_catch_)return;
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
        undo_manager_->Push();
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
        undo_manager_->Push();
    }
    ImGui::Separator();
    ImGui::Text("Scale:    %.2f, %.2f, %.2f", t->scale.x,
                t->scale.y, t->scale.z);
    if (ImGui::DragFloat3("Scale", &t->scale.x, kDragSpeed))
    {
    }
    if (ImGui::IsItemActivated())
    {
        undo_manager_->Push();
    }
    ImGui::Separator();
    ImGui::End();

}
