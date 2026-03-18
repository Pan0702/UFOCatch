#pragma once
#include <string>

class JsonReader
{
public:
    /// <summary>
    /// JSON繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ縺ｿ縲∬ｨ倬鹸縺輔ｌ縺溘Δ繝・Ν繧偵せ繝・・繧ｸ縺ｫ蠕ｩ蜈・☆繧九・
    /// 譛ｪ繝ｭ繝ｼ繝峨・繝｢繝・Ν縺ｯ閾ｪ蜍慕噪縺ｫ繝ｭ繝ｼ繝峨＠縺ｦ繝懊ち繝ｳ縺ｫ繧りｿｽ蜉縺吶ｋ縲・
    /// </summary>
    /// <param name="path">繧､繝ｳ繝昴・繝医☆繧徽SON繝輔ぃ繧､繝ｫ縺ｮ繝代せ</param>
    static void ImportModelData(const std::string& path);
};
