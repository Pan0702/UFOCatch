#pragma once
#include "../Common/Object3D.h"
class Camera : public Object3D
{
public:
    Camera();
    ~Camera();
    void Update() override;

    /// <summary>驕ｸ謚槭が繝悶ず繧ｧ繧ｯ繝医↓繧ｫ繝｡繝ｩ繧偵ヵ繧ｩ繝ｼ繧ｫ繧ｹ縺吶ｋ</summary>
    static void Focus();

    /// <summary>WASD繧ｭ繝ｼ縺ｧ繧ｫ繝｡繝ｩ繧貞燕蠕悟ｷｦ蜿ｳ縺ｫ蟷ｳ陦檎ｧｻ蜍輔☆繧・/summary>
    static void Move();

    /// <summary>繝槭え繧ｹ遘ｻ蜍暮㍼縺ｫ蠢懊§縺ｦ繧ｫ繝｡繝ｩ縺ｮ豕ｨ隕也せ繧貞屓霆｢縺吶ｋ</summary>
    static void Rotate();

    /// <summary>繝槭え繧ｹ繝帙う繝ｼ繝ｫ縺ｧ繧ｫ繝｡繝ｩ繧貞燕蠕梧婿蜷代↓繧ｺ繝ｼ繝縺吶ｋ</summary>
    static void Zoom();
};
