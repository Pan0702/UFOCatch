#include "AudioManager.h"
#include <unordered_map>

namespace {
    std::unordered_map<std::string, CXAudioSource*> audioFiles;
}

void AudioManager::Init()
{
    audioFiles.clear();
}

void AudioManager::Reset()
{
    for (auto& pair : audioFiles) {
        delete pair.second;
    }
    audioFiles.clear();
}

CXAudioSource* AudioManager::Load(const std::string& name, const std::string& filepath, DWORD sourceNum)
{
    if (audioFiles.find(name) == audioFiles.end()) {
        CXAudioSource* audio = new CXAudioSource();
        audio->Load(filepath.c_str(), sourceNum);
        audioFiles[name] = audio;
    }
    return audioFiles[name];
}

void AudioManager::Play(const std::string& name, bool loop)
{
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        it->second->Play(loop ? AUDIO_LOOP : 0);
    }
}

void AudioManager::Stop(const std::string& filename)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end()) {
        it->second->Stop();
    }
}

void AudioManager::StopAll()
{
    for (auto& pair : audioFiles) {
        pair.second->Stop();
    }
}

void AudioManager::SetVolume(const std::string& filename, float volume)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end()) {
        it->second->Volume(volume);
    }
}