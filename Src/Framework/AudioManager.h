#pragma once
#include <string>
#include <tchar.h>
#include "../Core/Graphics/XAudio.h"

using tstring = std::basic_string<TCHAR>;

namespace AudioManager
{
    /// 初期化する
    void Init();
    /// 初期状態に戻す
    void Reset();

    // @param filepath ファイルパス
    /// 読み込む
    /// @param name 名前
    /// @param filepath ファイルパス
    /// @param sourceNum sourceNum に渡す値
    /// @return 対象のポインタ
    CXAudioSource* Load(const tstring& name, const tstring& filepath, DWORD sourceNum = 1);

    /// 再生する
    /// @param name 名前
    /// @param loop ループ再生するか
    void Play(const tstring& name, bool loop = false);

    // 蛛懈ｭ｢
    /// 停止する
    /// @param name 名前
    void Stop(const tstring& name);

    // 全て停止
    /// All を停止する
    void StopAll();

    /// Volume を設定する
    /// @param name 名前
    /// @param volume 音量
    void SetVolume(const tstring& name, float volume);
}

namespace AudioStorage
{
    /// Music を初期化する
    void InitMusic();
}
