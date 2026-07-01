#include "AudioManager.h"
#include <unordered_map>

namespace
{
    std::unordered_map<tstring, CXAudioSource*> audioFiles;
}


void AudioManager::Init()
{
    audioFiles.clear();
}

// 全音声リソースを解放してマップをクリア
void AudioManager::Reset()
{
    for (auto& pair : audioFiles)
    {
        delete pair.second;
    }
    audioFiles.clear();
}

// 音声ファイルをロードして名前で登録する
CXAudioSource* AudioManager::Load(const tstring& name, const tstring& filepath, DWORD sourceNum)
{
    if (audioFiles.find(name) == audioFiles.end())
    {
        CXAudioSource* audio = new CXAudioSource();
        audio->Load(filepath.c_str(), sourceNum);
        audioFiles[name] = audio;
    }
    return audioFiles[name];
}

void AudioManager::Play(const tstring& name, bool loop)
{
    auto it = audioFiles.find(name);
    if (it != audioFiles.end())
    {
        it->second->Play(loop ? AUDIO_LOOP : 0);
    }
}

void AudioManager::Stop(const tstring& filename)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end())
    {
        it->second->Stop();
    }
}

void AudioManager::StopAll()
{
    for (auto& pair : audioFiles)
    {
        pair.second->Stop();
    }
}

void AudioManager::SetVolume(const tstring& filename, float volume)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end())
    {
        it->second->Volume(volume);
    }
}

void AudioStorage::InitMusic()
{
    AudioManager::Load("Play", _T("data/Sound/yukai.wav"));
    AudioManager::Load("Select", _T("data/Sound/himitu.wav"));
    AudioManager::Load("Title", _T("data/Sound/Sunny_day.wav"));
    AudioManager::Load("Decide", _T("data/Sound/decide.wav"));
    AudioManager::Load("Select", _T("data/Sound/select_002.wav"));
}
