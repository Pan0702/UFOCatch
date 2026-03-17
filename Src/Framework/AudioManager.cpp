#include "AudioManager.h"
#include <unordered_map>

namespace {
    // 逋ｻ骭ｲ蜷阪ｒ繧ｭ繝ｼ縺ｫ髻ｳ螢ｰ繧ｽ繝ｼ繧ｹ繧堤ｮ｡逅・☆繧九・繝・・
    std::unordered_map<tstring, CXAudioSource*> audioFiles;
}


// 繝槭ャ繝励ｒ繧ｯ繝ｪ繧｢・医Μ繧ｽ繝ｼ繧ｹ縺ｮ隗｣謾ｾ縺ｯ陦後ｏ縺ｪ縺・ｼ・
void AudioManager::Init()
{
    audioFiles.clear();
}

// 蜈ｨ髻ｳ螢ｰ繝ｪ繧ｽ繝ｼ繧ｹ繧定ｧ｣謾ｾ縺励※繝槭ャ繝励ｒ繧ｯ繝ｪ繧｢
void AudioManager::Reset()
{
    for (auto& pair : audioFiles) {
        delete pair.second;
    }
    audioFiles.clear();
}

// 髻ｳ螢ｰ繝輔ぃ繧､繝ｫ繧偵Ο繝ｼ繝峨＠縺ｦ蜷榊燕縺ｧ逋ｻ骭ｲ縺吶ｋ
// 蜷後§蜷榊燕縺梧里縺ｫ逋ｻ骭ｲ貂医∩縺ｮ蝣ｴ蜷医・繝ｭ繝ｼ繝峨○縺壹◎縺ｮ縺ｾ縺ｾ霑斐☆
CXAudioSource* AudioManager::Load(const tstring& name, const tstring& filepath, DWORD sourceNum)
{
    if (audioFiles.find(name) == audioFiles.end()) {
        CXAudioSource* audio = new CXAudioSource();
        audio->Load(filepath.c_str(), sourceNum);
        audioFiles[name] = audio;
    }
    return audioFiles[name];
}

// 謖・ｮ壹＠縺溷錐蜑阪・髻ｳ螢ｰ繧貞・逕溘☆繧・
// loop=true 縺ｧ繝ｫ繝ｼ繝怜・逕・
void AudioManager::Play(const tstring& name, bool loop)
{
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        it->second->Play(loop ? AUDIO_LOOP : 0);
    }
}

// 謖・ｮ壹＠縺溷錐蜑阪・髻ｳ螢ｰ繧貞●豁｢縺吶ｋ
void AudioManager::Stop(const tstring& filename)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end()) {
        it->second->Stop();
    }
}

// 逋ｻ骭ｲ縺輔ｌ縺ｦ縺・ｋ蜈ｨ髻ｳ螢ｰ繧貞●豁｢縺吶ｋ
void AudioManager::StopAll()
{
    for (auto& pair : audioFiles) {
        pair.second->Stop();
    }
}

// 謖・ｮ壹＠縺溷錐蜑阪・髻ｳ螢ｰ縺ｮ繝懊Μ繝･繝ｼ繝繧定ｨｭ螳壹☆繧具ｼ・.0f 縺梧ｨ呎ｺ厄ｼ・
void AudioManager::SetVolume(const tstring& filename, float volume)
{
    auto it = audioFiles.find(filename);
    if (it != audioFiles.end()) {
        it->second->Volume(volume);
    }
}

// 繧ｲ繝ｼ繝縺ｧ菴ｿ逕ｨ縺吶ｋBGM繝ｻSE繧偵∪縺ｨ繧√※繝ｭ繝ｼ繝峨☆繧・
void AudioStorage::InitMusic()
{
    AudioManager::Load("Play",   _T("data/Sound/yukai.wav"));
    AudioManager::Load("Select", _T("data/Sound/himitu.wav"));
    AudioManager::Load("Title",  _T("data/Sound/Sunny_day.wav"));
    AudioManager::Load("Decide", _T("data/Sound/decide.wav"));
    AudioManager::Load("Select", _T("data/Sound/select_002.wav"));
}
