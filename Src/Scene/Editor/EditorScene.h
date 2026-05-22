#pragma once
#include "../../Framework/SceneBase.h"

/// <summary>シーンで使う Editor Scene の情報と処理をまとめる型</summary>
class CEditorScene : public SceneBase
{
public:
    /// CEditorScene を初期化する
    CEditorScene();
    /// CEditorScene の終了処理を行う
    ~CEditorScene();
};
