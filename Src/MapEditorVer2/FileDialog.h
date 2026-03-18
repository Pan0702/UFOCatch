#pragma once
#include <string>

namespace Platform
{
    /// <summary>
    /// Windows縺ｮ繝輔ぃ繧､繝ｫ繧ｪ繝ｼ繝励Φ繝繧､繧｢繝ｭ繧ｰ繧定｡ｨ遉ｺ縺励・∈謚槭＆繧後◆繝輔ぃ繧､繝ｫ縺ｮ繝代せ繧定ｿ斐☆
    /// </summary>
    /// <param name="filter">繝輔ぃ繧､繝ｫ繝輔ぅ繝ｫ繧ｿ繝ｼ・井ｾ・ L"*.mesh"・・/param>
    /// <returns>驕ｸ謚槭＆繧後◆繝輔ぃ繧､繝ｫ縺ｮ繝輔Ν繝代せ・・TF-8・峨ゅく繝｣繝ｳ繧ｻ繝ｫ譎ゅ・遨ｺ譁・ｭ怜・</returns>
    std::string OpenFileDialog(const wchar_t* filter = L"All Files (*.*)\0*.*\0");

    /// <summary>
    /// Windows縺ｮ繝輔ぃ繧､繝ｫ繧ｻ繝ｼ繝悶ム繧､繧｢繝ｭ繧ｰ繧定｡ｨ遉ｺ縺励・∈謚槭＆繧後◆繝輔ぃ繧､繝ｫ縺ｮ繝代せ繧定ｿ斐☆
    /// </summary>
    /// <param name="filter">繝輔ぃ繧､繝ｫ繝輔ぅ繝ｫ繧ｿ繝ｼ・井ｾ・ L"*.mesh"・・/param>
    /// <returns>驕ｸ謚槭＆繧後◆繝輔ぃ繧､繝ｫ縺ｮ繝輔Ν繝代せ・・TF-8・峨ゅく繝｣繝ｳ繧ｻ繝ｫ譎ゅ・遨ｺ譁・ｭ怜・</returns>
    std::string SaveFileDialog(const wchar_t* filter = L"All Files (*.*)\0*.*\0", const wchar_t* initial_dir = nullptr);
}


