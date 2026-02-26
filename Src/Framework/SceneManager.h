#pragma once
/// <summary>
/// シーンの切り替えを管理するクラス
/// 実際のシーン作成ロジックは、SceneFactoryに記述してください
/// </summary>
/// <author>N.Hanai</author>
#include <string>
#include <list>
#include <memory>

class SceneFactory;
class SceneBase;
class CScreenTransition;

/// <summary>
/// 現在のシーンを呼び出し実行する。
/// シーンを切り替える際は、ChangeScene()を呼び出せばよく、
/// 呼んだ瞬間に切り替わるのではなく、次のUpdate()のタイミングで切り替わるように設計されている。
/// </summary>
namespace SceneManager {
	void Start();
	void Update();
	void Draw();
	void DrawTransition();
	void Release();

	/// <summary>
	/// 現在のシーンを取得する
	/// </summary>
	SceneBase* CurrentScene();

	/// <summary>
	/// 現在のシーンとして登録する
	/// </summary>
	/// <param name="scene">登録するシーンのインスタンス</param>
	void SetCurrentScene(std::unique_ptr<SceneBase> scene);

	/// <summary>
	/// シーンを切り替える。
	/// 即座には切り替わらず、次にUpdate()が呼ばれた際、Sceneが更新される前に切り替わります。
	/// シーンの名前とクラスの関係は、SceneFactory()に記述してください。
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// トランジション付きでシーンを切り替える
	/// </summary>
	void ChangeSceneWithTransition(const std::string& sceneName);

	/// <summary>
	/// トランジション中かどうか
	/// </summary>
	bool IsTransitioning();

	/// <summary>
	/// 前のフレームからの経過時間（秒）
	/// 通常、60フレームの場合は 0.01667 になるが、処理落ち等で2フレームかかれば 0.0333 になる。
	/// 最初の10フレーム程度は基準となる時間計算を行っているため、安定するまでは値が不規則になる。
	/// </summary>
	/// <returns>経過時間（秒）</returns>
	float DeltaTime();
    
	void Exit();
};