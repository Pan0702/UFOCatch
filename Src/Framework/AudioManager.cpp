#include "AudioManager.h"
#include <unordered_map>

namespace {
    // 登録名をキーに音声ソースを管理するマップ
    std::unordered_map<tstring, CXAudioSource*> audioFiles;
}


// マップをクリア（リソースの解放は行わない）
void AudioManager::Init()
{
    audioFiles.clear();
}

// 全音声リソースを解放してマップをクリア
void AudioManager::Reset()
{
    for (auto& pair : audioFiles) {
        delete pair.second;
    }
    audioFiles.clear();
}

// 音声ファイルをロードして名前で登録する
// 同じ名前が既に登録済みの場合はロードせずそのまま返す
CXAudioSource* AudioManager::Load(const tstring& name, const tstring& filepath, DWORD sourceNum)
{
    if (audioFiles.find(name) == audioFiles.end()) {
        CXAudioSource* audio = new CXAudioSource();
        audio->Load(filepath.c_str(), sourceNum);
        audioFiles[name] = audio;
    }
    return audioFiles[name];
}

// 指定した名前の音声を再生する
// loop=true でループ再生
void AudioManager::Play(const tstring& name, bool loop)
{
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        it->second->Play(loop ? AUDIO_LOOP : 0);
    }
}

// 指定した名前の音声を停止する
void AudioManager::Stop(const tstring& filename)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end()) {
        it->second->Stop();
    }
}

// 登録されている全音声を停止する
void AudioManager::StopAll()
{
    for (auto& pair : audioFiles) {
        pair.second->Stop();
    }
}

// 指定した名前の音声のボリュームを設定する（1.0f が標準）
void AudioManager::SetVolume(const tstring& filename, float volume)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end()) {
        it->second->Volume(volume);
    }
}

// ゲームで使用するBGM・SEをまとめてロードする
void AudioStorage::InitMusic()
{
    AudioManager::Load("Play",   _T("data/Sound/yukai.wav"));
    AudioManager::Load("Select", _T("data/Sound/himitu.wav"));
    AudioManager::Load("Title",  _T("data/Sound/Sunny_day.wav"));
    AudioManager::Load("Decide", _T("data/Sound/decide.wav"));
    AudioManager::Load("Select", _T("data/Sound/select_002.wav"));
}
