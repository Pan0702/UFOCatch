#pragma once
#include "UIButton.h"
#include "../Common/Constants.h"
#include "ImageRegistry.h"
#include "UICanvas.h"
#include "UIPreset.h"

const VECTOR4 NonImage = VECTOR4(0, 0, 0, 0);

/// @brief シーン単位のUIの基底クラス
/// @details CUICanvasを1つ持ち、Update/Drawをキャンバスに委譲する。
///          各シーンのUIクラスはこれを継承して使う。
class CUIBase
{
public:
    /// Canvas を取得する
    /// @return 処理結果
    CUICanvas& GetCanvas();
    /// 毎フレームの状態を更新する
    virtual void Update();
    /// 描画する
    virtual void Draw();

protected:
    CUICanvas m_canvas;
};
