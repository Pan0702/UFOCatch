#pragma once
/// <summary>
/// SceneManagerから呼び出されSceneを作成するクラス
/// SceneManagerが肥大化しないように、作成ロジックをここに分離している
/// 各シーンは、文字列（名前）で指定して作成できるようにする
/// </summary>
#include <memory>
#include <string>

class SceneBase;

class SceneFactory {
public:
	/// <summary>
	/// 最初に実行するシーン（タイトル画面など）を作成する
	/// </summary>
	/// <returns>作成したシーンのインスタンス</returns>
	static std::unique_ptr<SceneBase> CreateFirst();

	/// <summary>
	/// 名前を指定して特定のシーンを作成する
	/// </summary>
	/// <param name="name">シーンの名称</param>
	/// <returns>作成したシーンのインスタンス</returns>
	static std::unique_ptr<SceneBase> Create(const std::string& name);
};