#pragma once

namespace MainControl
{
    /// Use Refresh Message の処理を行う
    void UseRefreshMessage();

    /// Use Frame Timer の処理を行う
    /// @param time time に渡す値
    void UseFrameTimer(float time);

    /// Window Name を設定する
    /// @param name 名前
    void SetWindowName(const char* name);

    /// Refresh Timer を返す
    /// @return 計算結果の値
    float RefreshTimer();
};
