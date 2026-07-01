#pragma once

/// <summary>シーン基底クラスを定義するヘッダー</summary>
/// <author>N.Hanai</author>

#include <list>
#include "sceneManager.h"
#include "../Core/Game/GameMain.h"

/// <summary>各シーンが継承する共通インターフェース</summary>
class SceneBase
{
public:
    /// SceneBase を初期化する
    SceneBase();
    /// SceneBase の終了処理を行う
    virtual ~SceneBase();

    /// 毎フレームの状態を更新する
    virtual void Update()
    {
    }

    /// 描画する
    virtual void Draw()
    {
    }

    /// UIPtr を取得する
    /// @return 対象のポインタ
    CUIBase* GetUIPtr() const;

protected:
    CUIBase* m_pUI;
};
