#pragma once
#include "TRSBase.h"

class CTranslation : public TRSBase
{
private:
    /// <summary>X/Y/Z霆ｸ縺ｮ遘ｻ蜍輔ぐ繧ｺ繝｢繝｡繝・す繝･縺ｨ繧ｳ繝ｩ繧､繝繝ｼ繧貞・譛溷喧縺吶ｋ</summary>
    void InitMeshes();
    void Draw() override{}
public:
    CTranslation();
};

