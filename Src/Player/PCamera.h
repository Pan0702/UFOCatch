#pragma once
#include "../Common/Object3D.h"
#include "../Utils/LerpValue.h"

// プレイヤーを追従するカメラクラス //
class CPlayerCamera : public Object3D
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
    void ZoomOut(const VECTOR3& pos, const float& coneHeight);

private:
    //void DebugImGui();
    void Update() override;
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
    // ZoomIn逕ｨ縺ｮ蛻ｶ蠕｡轤ｹ・育峩謗･謖・ｮ夲ｼ・/
    VECTOR3 m_zoomInCtrl1 = VECTOR3(0, 5, -5); // 繧ｫ繝｡繝ｩ菴咲ｽｮ縺ｮ蛻ｶ蠕｡轤ｹ1//
    VECTOR3 m_zoomInCtrl2 = VECTOR3(0, 3, -6); // 繧ｫ繝｡繝ｩ菴咲ｽｮ縺ｮ蛻ｶ蠕｡轤ｹ2//
    VECTOR3 m_zoomInLookCtrl1 = VECTOR3(0, 1, 0); // 豕ｨ隕也せ縺ｮ蛻ｶ蠕｡轤ｹ1//
    VECTOR3 m_zoomInLookCtrl2 = VECTOR3(0, 0, 0); // 豕ｨ隕也せ縺ｮ蛻ｶ蠕｡轤ｹ2//

    // ZoomOut逕ｨ縺ｮ蛻ｶ蠕｡轤ｹ・育峩謗･謖・ｮ夲ｼ・/
    VECTOR3 m_zoomOutCtrl1 = VECTOR3(0, 5, -5);
    VECTOR3 m_zoomOutCtrl2 = VECTOR3(0, 6, -4);
    VECTOR3 m_zoomOutLookCtrl1 = VECTOR3(0, 1, 0);
    VECTOR3 m_zoomOutLookCtrl2 = VECTOR3(0, 1, -1);

    // 繝・ヰ繝・げ逕ｨ・壼ｧ狗せ繝ｻ邨らせ縺ｮ險倬鹸//
    VECTOR3 m_debugStartLook = VECTOR3(0, 0, 0);
    VECTOR3 m_debugTargetLook = VECTOR3(0, 0, 0);
#endif
};
