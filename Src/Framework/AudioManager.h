#pragma once
#include <string>
#include "../Core/Graphics/XAudio.h"

namespace AudioManager {
    void Init();
    void Reset();

    // 音声をロード（名前を付けて登録）
    // @param name 登録名（再生時に使う名前）
    // @param filepath ファイルパス
    // @param sourceNum 同時再生数（効果音の重ね再生用、デフォルト1）
    CXAudioSource* Load(const std::string& name, const std::string& filepath, DWORD sourceNum = 1);

    // 再生
    void Play(const std::string& name, bool loop = false);

    // 停止
    void Stop(const std::string& name);

    // 全て停止
    void StopAll();

    // ボリューム設定（1.0fが標準）
    void SetVolume(const std::string& name, float volume);
};