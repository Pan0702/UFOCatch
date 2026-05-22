#pragma once
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "../../Common/Object3D.h"


/// <summary>ステージエディタで使う Stage Data の情報と処理をまとめる型</summary>
class StageData;

/// <summary>ステージエディタで使う TRS の情報と処理をまとめる型</summary>
class TRS : public Object3D
{
private:
    std::unique_ptr<CTranslation> m_pTranslation_;
    std::unique_ptr<CRotation> m_pRotation;
    std::unique_ptr<CScaling> m_pScaling;
    StageData* m_pStageData = nullptr;
    Transform* m_pOverrideTarget = nullptr;
    Axis m_draggingAxis = Axis::None;

    int m_state = 0;
    int m_selected = 0;
    float m_translateSpeed = 0.1f;
    float m_rotateSpeed = 1.0f;
    float m_scaleSpeed = 1.0f;
    float m_translateAccum = 0.0f;
    float m_rotateAccum = 0.0f;

public:
    /// Panel を描画する
    void DrawPanel();

private:
    /// Radio Translate の処理を行う
    void RadioTranslate();

    /// Radio Rotate の処理を行う
    void RadioRotate();

    /// Radio Scale の処理を行う
    void RadioScale();

    /// Snap Translation の処理を行う
    /// @param compo compo に渡す値
    /// @param delta 変化量
    void SnapTranslation(float& compo, float delta);

    /// Snap Rotation の処理を行う
    /// @param compo compo に渡す値
    /// @param delta 変化量
    void SnapRotation(float& compo, float delta);

    /// Target を取得する
    /// @return 対象のポインタ
    Transform* GetTarget() const;

public:
    TRS();

    /// State を設定する
    /// @param state 状態
    void SetState(int state) { m_state = state; }

    /// 描画する
    void Draw() override;
    /// 毎フレームの状態を更新する
    void Update() override;

    /// Ray Hit Test を返す
    /// @param ray 判定に使用するレイ
    /// @return 処理結果
    Axis RayHitTest(const Ray& ray) const;

    /// Transform を設定する
    void SetTransform();

    /// Transform を追加する
    /// @param axis axis に渡す値
    /// @param objPos 座標
    /// @return 計算結果の値
    static float AddTransform(Axis axis, const VECTOR3& objPos);

    /// Dragging Axis を設定する
    /// @param axis axis に渡す値
    void SetDraggingAxis(Axis axis) { m_draggingAxis = axis; }

    /// Override Target を設定する
    /// @param t t に渡す値
    void SetOverrideTarget(Transform* t);

    /// <summary>State で扱う状態や種別を表す列挙型</summary>
    enum State : uint8_t
    {
        kNone,
        kTranslation,
        kRotation,
        kScaling
    };
};
