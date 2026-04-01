#pragma once
#include <string>
#include <tchar.h>
#include "../Core/Graphics/XAudio.h"

// TCHAR蟇ｾ蠢懊・string蝙・
using tstring = std::basic_string<TCHAR>;

namespace AudioManager {
    void Init();
    void Reset();

    // 髻ｳ螢ｰ繧偵Ο繝ｼ繝会ｼ亥錐蜑阪ｒ莉倥￠縺ｦ逋ｻ骭ｲ・・
    // @param name 逋ｻ骭ｲ蜷搾ｼ亥・逕滓凾縺ｫ菴ｿ縺・錐蜑搾ｼ・
    // @param filepath 繝輔ぃ繧､繝ｫ繝代せ
    // @param sourceNum 蜷梧凾蜀咲函謨ｰ・亥柑譫憺浹縺ｮ驥阪・蜀咲函逕ｨ縲√ョ繝輔か繝ｫ繝・・・
    CXAudioSource* Load(const tstring& name, const tstring& filepath, DWORD sourceNum = 1);

    // 蜀咲函
    void Play(const tstring& name, bool loop = false);

    // 蛛懈ｭ｢
    void Stop(const tstring& name);

    // 蜈ｨ縺ｦ蛛懈ｭ｢
    void StopAll();

    // 繝懊Μ繝･繝ｼ繝險ｭ螳夲ｼ・.0f縺梧ｨ呎ｺ厄ｼ・
    void SetVolume(const tstring& name, float volume);
}

namespace AudioStorage
{
    void InitMusic();
}


