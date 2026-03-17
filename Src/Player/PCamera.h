#pragma once
#include "../Common/Object3D.h"
#include "../Utils/LerpValue.h"

// 繝励Ξ繧､繝､繝ｼ繧定ｿｽ蠕薙☆繧九き繝｡繝ｩ繧ｯ繝ｩ繧ｹ //
class CPlayerCamera:public Object3D
{
public:
    CPlayerCamera();
    ~CPlayerCamera();
    // 繧ｫ繝｡繝ｩ菴咲ｽｮ繧定ｨｭ螳壹☆繧・
    // @param pos 繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ
    // @param distance 繧ｳ繝ｼ繝ｳ縺ｮ鬮倥＆ //
    void PosSet(const VECTOR3& pos, const float& distance);
    // 繧ｫ繝｡繝ｩ繧偵ぜ繝ｼ繝繧､繝ｳ縺輔○繧・
    // @param pos 繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ //
    void ZoomIn(const VECTOR3& pos);
    // 繧ｫ繝｡繝ｩ繧偵ぜ繝ｼ繝繧｢繧ｦ繝医＆縺帙ｋ
    // @param pos 繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ //
    void ZoomOut(const VECTOR3& pos);
private:
    //void DebugImGui();
    void Update() override;
    void UpdateCameraBezier();


    VECTOR3 m_camPos;
    VECTOR3 m_camLook;
    
    BezierValueVec3 m_camPosBezier;
    LerpValueVec3 m_camLookBezier;
    uint8_t state;

    VECTOR3 m_animStartPlayerPos;  // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ髢句ｧ区凾縺ｮ繝励Ξ繧､繝､繝ｼ菴咲ｽｮ
    VECTOR3 m_playerOffset;        // 繝励Ξ繧､繝､繝ｼ遘ｻ蜍輔↓繧医ｋ蟾ｮ蛻・
    enum : uint8_t
    {
        zoomIn = 0,
        zoomOut = 1,
    };
    
#if 0
    // ZoomIn逕ｨ縺ｮ蛻ｶ蠕｡轤ｹ・育峩謗･謖・ｮ夲ｼ・/
    VECTOR3 m_zoomInCtrl1 = VECTOR3(0, 5, -5);   // 繧ｫ繝｡繝ｩ菴咲ｽｮ縺ｮ蛻ｶ蠕｡轤ｹ1//
    VECTOR3 m_zoomInCtrl2 = VECTOR3(0, 3, -6);   // 繧ｫ繝｡繝ｩ菴咲ｽｮ縺ｮ蛻ｶ蠕｡轤ｹ2//
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
