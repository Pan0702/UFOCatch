#pragma once
#include "TRSBase.h"
class CScaling : public TRSBase
{
private:
    /// <summary>X/Y/Z霆ｸ縺ｮ繧ｹ繧ｱ繝ｼ繝ｫ繧ｮ繧ｺ繝｢繝｡繝・す繝･縺ｨ繧ｳ繝ｩ繧､繝繝ｼ繧貞・譛溷喧縺吶ｋ</summary>
    void InitMeshes();

public:
    CScaling();
    ~CScaling();
    void Draw() override{}
};

