#pragma once
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "../../Object3D.h"


class StageData;

class TRS : public Object3D
{
private:
    std::unique_ptr<CTranslation> translation_;
    std::unique_ptr<CRotation> rotation_;
    std::unique_ptr<CScaling> scaling_;
    StageData* stage_data_ = nullptr;
    Transform* override_target_ = nullptr;
    Axis dragging_axis_ = Axis::None;

    int state_ = 0;
    int selected_ = 0;
    float translate_speed_ = 0.1f;
    float rotate_speed_ = 1.0f;
    float scale_speed_ = 1.0f;
    float translate_accum_ = 0.0f;
    float rotate_accum_ = 0.0f;
private:
    /// <summary>ImGuiで移動スナップ量を選択するラジオボタンを描画する</summary>
    void RadioTranslate();

    /// <summary>ImGuiでTRSモード切替と各設定UIをまとめて描画する</summary>
    void DrawImGui();

    /// <summary>ImGuiで回転スナップ量を選択するラジオボタンを描画する</summary>
    void RadioRotate();

    /// <summary>ImGuiでスケールスピードを選択するラジオボタンを描画する</summary>
    void RadioScale();

    /// <summary>移動量をスナップ単位に丸めてcompoに累積加算する</summary>
    /// <param name="compo">更新対象の座標成分（参照）</param>
    /// <param name="delta">今フレームの移動量</param>
    void SnapTranslation(float& compo, float delta);

    /// <summary>回転量をスナップ単位に丸めてcompoに累積加算する</summary>
    /// <param name="compo">更新対象の回転成分（参照）</param>
    /// <param name="delta">今フレームの回転量</param>
    void SnapRotation(float& compo, float delta);
    
    Transform* GetTarget();

public:
    TRS();

    /// <summary>TRSの操作モードを設定する（State列挙を参照）</summary>
    /// <param name="state">設定するState値</param>
    void SetState(int state) { state_ = state; }

    void Draw() override;
    void Update() override;

    /// <summary>現在のモードに応じたギズモとレイの当たり判定を行い、当たった軸を返す</summary>
    /// <param name="ray">判定に使用するレイ</param>
    /// <returns>当たった軸（X/Y/Z）、当たらなければNone</returns>
    Axis RayHitTest(const Ray& ray) const;

    /// <summary>ドラッグ中の軸に沿って選択オブジェクトのTransformを更新する</summary>
    void SetTransform();

    /// <summary>マウス移動量を指定軸のスクリーン投影ベクトルに射影し、ワールド空間の変化量を返す</summary>
    /// <param name="axis">操作軸</param>
    /// <param name="objPos">対象オブジェクトのワールド座標</param>
    /// <returns>軸方向の変化量</returns>
    static float AddTransform(Axis axis, const VECTOR3& objPos);

    /// <summary>ドラッグ中の軸を設定する。Noneを指定するとドラッグを解除する</summary>
    /// <param name="axis">設定する軸</param>
    void SetDraggingAxis(Axis axis) { dragging_axis_ = axis; }

    void SetOverrideTarget(Transform* t);
    
    enum State : uint8_t
    {
        kNone,
        kTranslation,
        kRotation,
        kScaling
    };
};
