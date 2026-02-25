#pragma once

namespace MainControl {
	/// <summary>
	/// フレームレート（描画更新）にウィンドウメッセージを利用するように設定する
	/// (垂直同期など、OS側の更新タイミングに合わせる場合に使用)
	/// </summary>
	void UseRefreshMessage();

	/// <summary>
	/// フレームレートを固定するためのタイマーを使用するように設定する
	/// 1フレームの時間を秒で指定するため、60fpsを目指す場合は
	/// 1.0 / 60.0 を指定する
	/// </summary>
	/// <param name="time">更新間隔（秒）</param>
	void UseFrameTimer(float time);

	/// <summary>
	/// ウィンドウの名前を変更する
	/// 名前は、ウィンドウのタイトルバーに表示されます
	/// </summary>
	/// <param name="name">ウィンドウの名称</param>
	void SetWindowName(const char* name);

	/// <summary>
	/// 現在設定されているリフレッシュタイマーの値を取得する
	/// 0.0以下の場合は、RefreshMessage（メッセージ同期）が使用されていることを示す
	/// </summary>
	/// <returns>フレームタイマーの設定値（秒）</returns>
	float RefreshTimer();
};