#pragma once
#include "TRSBase.h"

class CTranslation : public TRSBase
{
private:
    /// <summary>X/Y/Z軸の移動ギズモメッシュとコライダーを初期化する</summary>
    void InitMeshes();
    void Draw() override{}
public:
    CTranslation();
};
