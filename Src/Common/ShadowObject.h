#pragma once
#include "../Framework/GameObject.h"
#include "../Utils/Sprite3D.h"

class Object3D;

/// <summary>
/// 繧ｪ繝悶ず繧ｧ繧ｯ繝医・荳九↓蠖ｱ繧呈緒逕ｻ縺吶ｋ繧ｯ繝ｩ繧ｹ
/// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ縺ｧ貂｡縺励◆繧ｪ繝ｼ繝翫・縺ｮ菴咲ｽｮ繝ｻ蝗櫁ｻ｢繧貞盾辣ｧ縺励※謠冗判縺吶ｋ
/// SetDrawOrder() 縺ｧ謠冗判繧ｿ繧､繝溘Φ繧ｰ繧堤峡遶九＠縺ｦ蛻ｶ蠕｡縺ｧ縺阪ｋ
/// </summary>
class CShadowObject : public GameObject {
public:
    CShadowObject(Object3D* pOwner, const TCHAR* imagePath, int drawOrder = -5);
    ~CShadowObject();
    void Draw() override;
    void DestroyMe() override;

private:
    Object3D*     m_pOwner;
    CSpriteImage* m_pSpriteImage;
};
