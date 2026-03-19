#pragma once
#include "TRSBase.h"
class CScaling : public TRSBase
{
private:
    /// <summary>X/Y/Z軸のスケールギズモメッシュとコライダーを初期化する</summary>
    void InitMeshes();

public:
    CScaling();
    ~CScaling();
    void Draw() override{}
};
