#pragma once
/// <summary>シーン管理機能を定義するヘッダー</summary>
/// <author>N.Hanai</author>
#include <string>
#include <list>
#include <memory>

#include "../UI/UIBase.h"

/// <summary>ゲーム共通基盤で使う Scene Factory の情報と処理をまとめる型</summary>
class SceneFactory;
/// <summary>各シーンが継承する共通インターフェース</summary>
class SceneBase;
/// <summary>ゲーム共通基盤で使う Screen Transition の情報と処理をまとめる型</summary>
class CScreenTransition;

/// <summary>
/// 現在のシーンを呼び出して実行する
/// </summary>
namespace SceneManager
{
    /// 開始する
    void Start();
    /// 毎フレームの状態を更新する
    void Update();
    /// 描画する
    void Draw();
    /// Transition を描画する
    void DrawTransition();
    /// 解放する
    void Release();

    /// Current Scene を返す
    /// @return 対象のポインタ
    SceneBase* CurrentScene();

    /// Current Scene を設定する
    /// @param scene scene に渡す値
    void SetCurrentScene(std::unique_ptr<SceneBase> scene);

    /// Scene を切り替える
    /// @param sceneName シーン名
    void ChangeScene(const std::string& sceneName);

    /// Scene With Transition を切り替える
    /// @param sceneName シーン名
    void ChangeSceneWithTransition(const std::string& sceneName);

    /// Transitioning を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsTransitioning();

    /// Delta Time を返す
    /// @return 計算結果の値
    float DeltaTime();

    /// Exit の処理を行う
    void Exit();

    /// UIPtr を取得する
    /// @return 対象のポインタ
    CUIBase* GetUIPtr();

    /// Debug Input の処理を行う
    void DebugInput();

    /// Debug Scene Init の処理を行う
    void DebugSceneInit();
};
