#pragma once
#include "../Common/Object3D.h"
#include "../Utils/LerpValue.h"

// プレイヤーを追従するカメラクラス //
class CPlayerCamera:public Object3D
{
public:
    CPlayerCamera();
    ~CPlayerCamera();
    // カメラ位置を設定する
    // @param pos プレイヤーの位置
    // @param distance コーンの高さ //
    void PosSet(const VECTOR3& pos, const float& distance);
    // カメラをズームインさせる
    // @param pos プレイヤーの位置 //
    void ZoomIn(const VECTOR3& pos);
    // カメラをズームアウトさせる
    // @param pos プレイヤーの位置 //
    void ZoomOut(const VECTOR3& pos);
private:
    //void DebugImGui();
    void Update() override;
    void UpdateCameraBezier();


    VECTOR3 m_camPos;
    VECTOR3 m_camLook;

    
    BezierValueVec3 m_camPosBezier;
    LerpValueVec3 m_camLookBezier;
    int state;

    VECTOR3 m_animStartPlayerPos;  // アニメーション開始時のプレイヤー位置
    VECTOR3 m_playerOffset;        // プレイヤー移動による差分
    enum
    {
        zoomIn = 0,
        zoomOut = 1,
    };
    
#if 0
    // ZoomIn用の制御点（直接指定）//
    VECTOR3 m_zoomInCtrl1 = VECTOR3(0, 5, -5);   // カメラ位置の制御点1//
    VECTOR3 m_zoomInCtrl2 = VECTOR3(0, 3, -6);   // カメラ位置の制御点2//
    VECTOR3 m_zoomInLookCtrl1 = VECTOR3(0, 1, 0); // 注視点の制御点1//
    VECTOR3 m_zoomInLookCtrl2 = VECTOR3(0, 0, 0); // 注視点の制御点2//

    // ZoomOut用の制御点（直接指定）//
    VECTOR3 m_zoomOutCtrl1 = VECTOR3(0, 5, -5);
    VECTOR3 m_zoomOutCtrl2 = VECTOR3(0, 6, -4);
    VECTOR3 m_zoomOutLookCtrl1 = VECTOR3(0, 1, 0);
    VECTOR3 m_zoomOutLookCtrl2 = VECTOR3(0, 1, -1);

    // デバッグ用：始点・終点の記録//
    VECTOR3 m_debugStartLook = VECTOR3(0, 0, 0);
    VECTOR3 m_debugTargetLook = VECTOR3(0, 0, 0);
#endif

};
