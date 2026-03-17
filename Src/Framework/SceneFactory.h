#pragma once
/// <summary>
/// SceneManager縺九ｉ蜻ｼ縺ｳ蜃ｺ縺輔ｌScene繧剃ｽ懈・縺吶ｋ繧ｯ繝ｩ繧ｹ
/// SceneManager縺瑚ぇ螟ｧ蛹悶＠縺ｪ縺・ｈ縺・↓縲∽ｽ懈・繝ｭ繧ｸ繝・け繧偵％縺薙↓蛻・屬縺励※縺・ｋ
/// 蜷・す繝ｼ繝ｳ縺ｯ縲∵枚蟄怜・・亥錐蜑搾ｼ峨〒謖・ｮ壹＠縺ｦ菴懈・縺ｧ縺阪ｋ繧医≧縺ｫ縺吶ｋ
/// </summary>
#include <memory>
#include <string>

class SceneBase;

class SceneFactory {
public:
	/// <summary>
	/// 譛蛻昴↓螳溯｡後☆繧九す繝ｼ繝ｳ・医ち繧､繝医Ν逕ｻ髱｢縺ｪ縺ｩ・峨ｒ菴懈・縺吶ｋ
	/// </summary>
	/// <returns>菴懈・縺励◆繧ｷ繝ｼ繝ｳ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ</returns>
	static std::unique_ptr<SceneBase> CreateFirst();

	/// <summary>
	/// 蜷榊燕繧呈欠螳壹＠縺ｦ迚ｹ螳壹・繧ｷ繝ｼ繝ｳ繧剃ｽ懈・縺吶ｋ
	/// </summary>
	/// <param name="name">繧ｷ繝ｼ繝ｳ縺ｮ蜷咲ｧｰ</param>
	/// <returns>菴懈・縺励◆繧ｷ繝ｼ繝ｳ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ</returns>
	static std::unique_ptr<SceneBase> Create(const std::string& name);
};