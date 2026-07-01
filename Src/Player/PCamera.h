#pragma once
#include "../Common/Object3D.h"
#include "../Utils/LerpValue.h"

// プレイヤーを追従するカメラクラス //
class CPlayerCamera : public Object3D
{
public:
    /// CPlayerCamera を初期化する
    CPlayerCamera();
    /// CPlayerCamera の終了処理を行う
    ~CPlayerCamera();
    // カメラ位置を設定する
    // @param pos プレイヤーの位置
    // @param distance コーンの高さ //
    /// Pos Set の処理を行う
    /// @param pos 座標
    /// @param distance distance に渡す値
    void PosSet(const VECTOR3& pos, const float& distance);
    // カメラをズームインさせる
    // @param pos プレイヤーの位置 //
    /// Zoom In の処理を行う
    /// @param pos 座標
    void ZoomIn(const VECTOR3& pos);
    // カメラをズームアウトさせる
    // @param pos プレイヤーの位置 //
    /// Zoom Out の処理を行う
    /// @param pos 座標
    /// @param coneHeight 高さ
    void ZoomOut(const VECTOR3& pos, const float& coneHeight);

private:
    /// Debug Im Gui の処理を行う
    //void DebugImGui();
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Camera Bezier を毎フレームの状態を更新する
    void UpdateCameraBezier();

    VECTOR3 m_camPos;
    VECTOR3 m_camLook;

    BezierValueVec3 m_camPosBezier;
    LerpValueVec3 m_camLookBezier;
    uint8_t state;

    VECTOR3 m_animStartPlayerPos; // アニメーション開始時のプレイヤー位置
    VECTOR3 m_playerOffset; // プレイヤー移動による差分
    enum : uint8_t
    {
        zoomIn = 0,
        zoomOut = 1,
    };

#if 0
    VECTOR3 m_zoomInCtrl1 = VECTOR3(0, 5, -5); // カメラ位置の制御点1//
    VECTOR3 m_zoomInCtrl2 = VECTOR3(0, 3, -6); // カメラ位置の制御点2//
    VECTOR3 m_zoomInLookCtrl1 = VECTOR3(0, 1, 0); // 注視点の制御点1//
    VECTOR3 m_zoomInLookCtrl2 = VECTOR3(0, 0, 0); // 注視点の制御点2//

    VECTOR3 m_zoomOutCtrl1 = VECTOR3(0, 5, -5);
    VECTOR3 m_zoomOutCtrl2 = VECTOR3(0, 6, -4);
    VECTOR3 m_zoomOutLookCtrl1 = VECTOR3(0, 1, 0);
    VECTOR3 m_zoomOutLookCtrl2 = VECTOR3(0, 1, -1);

    VECTOR3 m_debugStartLook = VECTOR3(0, 0, 0);
    VECTOR3 m_debugTargetLook = VECTOR3(0, 0, 0);
#endif
};
