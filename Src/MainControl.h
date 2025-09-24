#pragma once

namespace MainControl {
	/// <summary>
	/// フレームレートは、描画更新のメッセージを使う
	/// </summary>
	void UseRefreshMessage();

	/// <summary>
	/// フレームレートを設定する
	/// １フレームの時間を秒で設定するので、６０ｆｐｓの場合は、
	/// 1.0/60.0を指定する
	/// </summary>
	/// <param name="time">更新時間（秒）</param>
	void UseFrameTimer(float time);

	/// <summary>
	/// ウィンドウの名前を変更する
	/// 名前は、ウィンドウの左上に表示されます
	/// </summary>
	/// <param name="name">ウィンドウの名前</param>
	void SetWindowName(const char* name);

	/// <summary>
	/// 現在設定されているリフレッシュタイマーを取得する
	/// 0.0以下の場合は、RefreshMessageを使ってください。
	/// </summary>
	/// <returns>フレームタイマー</returns>
	float RefreshTimer();
};