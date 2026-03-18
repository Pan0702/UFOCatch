#pragma once
#include "ComponentBase.h"

class CWalk : public CComponentBase
{
public:
    CWalk(CEnemyBase* e, float speed);
    void Enter() override;
    void Update() override;

private:
    
    /// 蠅・阜蜀・↓蜿弱∪繧九Λ繝ｳ繝繝縺ｪ蝗櫁ｻ｢驥上→遘ｻ蜍戊ｷ晞屬繧・逕滓・縺吶ｋ
    /// 繝ｩ繝ｳ繝繝縺ｫ蝗櫁ｻ｢驥擾ｼ・180ﾂｰ・・180ﾂｰ・峨→遘ｻ蜍戊ｷ晞屬  ・・ .0・・.5・峨ｒ逕滓・縺励・蠅・阜繝√ぉ繝・け縺ｫ騾壹ｋ縺ｾ縺ｧ譛螟ｧ50蝗槭Μ繝医Λ繧､縺吶ｋ縲・
    /// 譛牙柑縺ｪ邨・∩蜷医ｏ縺帙′隕九▽縺九▲縺溷ｴ蜷医［_turnAm  ount縺ｨm_moveAmount縺ｫ險ｭ螳壹＆繧後ｋ縲・
    /// @return 譛牙柑縺ｪ遘ｻ蜍輔ヱ繝ｩ繝｡繝ｼ繧ｿ縺瑚ｦ九▽縺九▲縺溷ｴ  蜷・rue縲・譛螟ｧ隧ｦ陦悟屓謨ｰ繧定ｶ・∴縺溷ｴ蜷・alse
    bool CalcRandomMove();
    
    /*
    ///  蝗櫁ｻ｢繝ｻ遘ｻ蜍募ｾ後・菴咲ｽｮ縺悟｢・阜蜀・↓蜿弱∪繧九°繝√ぉ繝・け
    /// @param areaSize 繧ｨ繝ｪ繧｢縺ｮ繧ｵ繧､繧ｺ
    /// @return 蠅・阜蜀・↑繧液rue縲∝｢・阜螟悶↑繧映alse
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    */
    ///Walk縺ｮ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧・.0f縺ｮ騾溷ｺｦ縺ｧ蜀咲函縺吶ｋ
    void PlayWalkAnimation();
    
    /// 隗貞ｺｦ繧池ad縺ｧ-ﾏ・槃縺ｮ髢薙↓豁｣隕丞喧縺吶ｋ
    /// @param angle 隗貞ｺｦ
    /// @return -ﾏ・槃縺ｮ髢薙↓豁｣隕丞喧縺励◆隗貞ｺｦ繧定ｿ斐☆
    static float ClampRotateY(float angle);
    

    VECTOR3 m_position;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    float m_turnAmount;
    float m_currentRotation;
    float m_targetRotation;
    float m_moveSpeed;
    bool m_rotation;
    static constexpr float kTurnAngleDeg = 180.0f; // 繝ｩ繝ｳ繝繝蝗櫁ｻ｢縺ｮ遽・峇・亥ｺｦ・峨・
};

