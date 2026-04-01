#pragma once

namespace MainControl {
	/// <summary>
	/// 繝輔Ξ繝ｼ繝繝ｬ繝ｼ繝茨ｼ域緒逕ｻ譖ｴ譁ｰ・峨↓繧ｦ繧｣繝ｳ繝峨え繝｡繝・そ繝ｼ繧ｸ繧貞茜逕ｨ縺吶ｋ繧医≧縺ｫ險ｭ螳壹☆繧・
	/// (蝙ら峩蜷梧悄縺ｪ縺ｩ縲＾S蛛ｴ縺ｮ譖ｴ譁ｰ繧ｿ繧､繝溘Φ繧ｰ縺ｫ蜷医ｏ縺帙ｋ蝣ｴ蜷医↓菴ｿ逕ｨ)
	/// </summary>
	void UseRefreshMessage();

	/// <summary>
	/// 繝輔Ξ繝ｼ繝繝ｬ繝ｼ繝医ｒ蝗ｺ螳壹☆繧九◆繧√・繧ｿ繧､繝槭・繧剃ｽｿ逕ｨ縺吶ｋ繧医≧縺ｫ險ｭ螳壹☆繧・
	/// 1繝輔Ξ繝ｼ繝縺ｮ譎る俣繧堤ｧ偵〒謖・ｮ壹☆繧九◆繧√・0fps繧堤岼謖・☆蝣ｴ蜷医・
	/// 1.0 / 60.0 繧呈欠螳壹☆繧・
	/// </summary>
	/// <param name="time">譖ｴ譁ｰ髢馴囈・育ｧ抵ｼ・/param>
	void UseFrameTimer(float time);

	/// <summary>
	/// 繧ｦ繧｣繝ｳ繝峨え縺ｮ蜷榊燕繧貞､画峩縺吶ｋ
	/// 蜷榊燕縺ｯ縲√え繧｣繝ｳ繝峨え縺ｮ繧ｿ繧､繝医Ν繝舌・縺ｫ陦ｨ遉ｺ縺輔ｌ縺ｾ縺・
	/// </summary>
	/// <param name="name">繧ｦ繧｣繝ｳ繝峨え縺ｮ蜷咲ｧｰ</param>
	void SetWindowName(const char* name);

	/// <summary>
	/// 迴ｾ蝨ｨ險ｭ螳壹＆繧後※縺・ｋ繝ｪ繝輔Ξ繝・す繝･繧ｿ繧､繝槭・縺ｮ蛟､繧貞叙蠕励☆繧・
	/// 0.0莉･荳九・蝣ｴ蜷医・縲ヽefreshMessage・医Γ繝・そ繝ｼ繧ｸ蜷梧悄・峨′菴ｿ逕ｨ縺輔ｌ縺ｦ縺・ｋ縺薙→繧堤､ｺ縺・
	/// </summary>
	/// <returns>繝輔Ξ繝ｼ繝繧ｿ繧､繝槭・縺ｮ險ｭ螳壼､・育ｧ抵ｼ・/returns>
	float RefreshTimer();
};
