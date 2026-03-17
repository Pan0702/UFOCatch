#pragma once
#include "TRSBase.h"
class CRotation: public TRSBase
{
private:
    /// <summary>X/Y/Z軸の回転ギズモメッシュとコライダーを初期化する</summary>
    void InitMeshes();

public:
    CRotation();
    ~CRotation();
    void Draw() override{}

};
