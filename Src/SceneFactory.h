#pragma once
/// <summary>
/// SceneManagerから呼び出されてSceneを作成するクラス
/// SceneManagerを触らなくていいように、作成部分をここに分離しておく
/// 作るシーンは、文字列で指定してもらうようにした
/// </summary>
#include <string>

class SceneBase;

class SceneFactory {
public:
	/// <summary>
	/// 最初にシーンを作る
	/// </summary>
	/// <returns>作成したインスタンス</returns>
	SceneBase* CreateFirst();

	/// <summary>
	/// 指定されたシーンを作成する
	/// </summary>
	/// <param name="name">シーンの名称</param>
	/// <returns>作成したインスタンス</returns>
	SceneBase* Create(const std::string& name);
};
