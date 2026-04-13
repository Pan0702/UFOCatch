#pragma once
#include "UIButton.h"
#include "../Common/Constants.h"
#include "ImageRegistry.h"
#include "UICanvas.h"

/// @brief シーン単位のUIの基底クラス
/// @details CUICanvasを1つ持ち、Update/Drawをキャンバスに委譲する。
///          各シーンのUIクラスはこれを継承して使う。
class CUIBase
{
public:
    CUICanvas& GetCanvas();
    virtual void Update();
    virtual void Draw();
protected:
    CUICanvas m_canvas;
};


