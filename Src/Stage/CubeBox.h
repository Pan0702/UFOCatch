#pragma once
#include "../Common/Object3D.h"

// 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ繧定｡ｨ遉ｺ縺吶ｋ繧ｯ繝ｩ繧ｹ //
class CCubeBox : public Object3D
{
public:
    // 繝｡繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ繧呈欠螳壹＠縺ｦ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
    // @param meshPath 繝｡繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ //
    CCubeBox(const char* meshPath);
    ~CCubeBox();
private:

    void Draw() override;
};

