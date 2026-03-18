#pragma once
#include "TRSBase.h"
class CRotation: public TRSBase
{
private:
    /// <summary>X/Y/Z霆ｸ縺ｮ蝗櫁ｻ｢繧ｮ繧ｺ繝｢繝｡繝・す繝･縺ｨ繧ｳ繝ｩ繧､繝繝ｼ繧貞・譛溷喧縺吶ｋ</summary>
    void InitMeshes();

public:
    CRotation();
    ~CRotation();
    void Draw() override{}

};

