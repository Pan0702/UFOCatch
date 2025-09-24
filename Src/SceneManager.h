#pragma once
/// <summary>
/// シーンの切り替えを管理するクラス
/// 実際のシーンの作成部分は、SceneFactoryに書いてください
/// </summary>
/// <author>N.Hanai</author>
#include <string>
#include <list>

class SceneFactory;
class SceneBase;

/// <summary>
/// 現在のシーンを呼び出している。
/// Sceneを切り替える時は、ChangeScene()を呼び出せばよく、
/// 呼んだ時に切り替えるのではなく、次回のUpdate()で切り替えるようにしている。
/// </summary>
namespace SceneManager {
	void Start();
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// 現在のシーンを取得する
	/// </summary>
	SceneBase* CurrentScene();

	/// <summary>
	/// 現在のシーンとして登録する
	/// </summary>
	/// <param name="scene"></param>
	void SetCurrentScene(SceneBase* scene);

	/// <summary>
	/// シーンを切り替える。
	/// 直接ここでは切り替えず、次回Update()が呼ばれたとき、Scene処理を呼ぶ前に切り替えます。
	/// シーンの名前とクラスの関係は、SceneFactory()に記述してください。
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// 前のフレームからの経過時間（秒）
	/// 通常、60フレームの場合は、0.01667になるが、前の処理が2フレームかかったら0.0333になる
	/// 直近10フレーム程度で、基準となる時間計測をしているので、安定するまでは、値が不安定になります。
	/// </summary>
	/// <returns></returns>
	float DeltaTime();

	void Exit();
};
