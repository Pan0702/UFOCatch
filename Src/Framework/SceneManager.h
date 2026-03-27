#pragma once
/// <summary>
/// 繧ｷ繝ｼ繝ｳ縺ｮ蛻・ｊ譖ｿ縺医ｒ邂｡逅・☆繧九け繝ｩ繧ｹ
/// 螳滄圀縺ｮ繧ｷ繝ｼ繝ｳ菴懈・繝ｭ繧ｸ繝・け縺ｯ縲ヾceneFactory縺ｫ險倩ｿｰ縺励※縺上□縺輔＞
/// </summary>
/// <author>N.Hanai</author>
#include <string>
#include <list>
#include <memory>

class SceneFactory;
class SceneBase;
class CScreenTransition;

/// <summary>
/// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ繧貞他縺ｳ蜃ｺ縺怜ｮ溯｡後☆繧九・
/// 繧ｷ繝ｼ繝ｳ繧貞・繧頑崛縺医ｋ髫帙・縲，hangeScene()繧貞他縺ｳ蜃ｺ縺帙・繧医￥縲・
/// 蜻ｼ繧薙□迸ｬ髢薙↓蛻・ｊ譖ｿ繧上ｋ縺ｮ縺ｧ縺ｯ縺ｪ縺上∵ｬ｡縺ｮUpdate()縺ｮ繧ｿ繧､繝溘Φ繧ｰ縺ｧ蛻・ｊ譖ｿ繧上ｋ繧医≧縺ｫ險ｭ險医＆繧後※縺・ｋ縲・
/// </summary>
namespace SceneManager {
	void Start();
	void Update();
	void Draw();
	void DrawTransition();
	void Release();

	/// <summary>
	/// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ繧貞叙蠕励☆繧・
	/// </summary>
	SceneBase* CurrentScene();

	/// <summary>
	/// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｨ縺励※逋ｻ骭ｲ縺吶ｋ
	/// </summary>
	/// <param name="scene">逋ｻ骭ｲ縺吶ｋ繧ｷ繝ｼ繝ｳ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ</param>
	void SetCurrentScene(std::unique_ptr<SceneBase> scene);

	/// <summary>
	/// 繧ｷ繝ｼ繝ｳ繧貞・繧頑崛縺医ｋ縲・
	/// 蜊ｳ蠎ｧ縺ｫ縺ｯ蛻・ｊ譖ｿ繧上ｉ縺壹∵ｬ｡縺ｫUpdate()縺悟他縺ｰ繧後◆髫帙ヾcene縺梧峩譁ｰ縺輔ｌ繧句燕縺ｫ蛻・ｊ譖ｿ繧上ｊ縺ｾ縺吶・
	/// 繧ｷ繝ｼ繝ｳ縺ｮ蜷榊燕縺ｨ繧ｯ繝ｩ繧ｹ縺ｮ髢｢菫ゅ・縲ヾceneFactory()縺ｫ險倩ｿｰ縺励※縺上□縺輔＞縲・
	/// </summary>
	/// <param name="sceneName">繧ｷ繝ｼ繝ｳ蜷・/param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ莉倥″縺ｧ繧ｷ繝ｼ繝ｳ繧貞・繧頑崛縺医ｋ
	/// </summary>
	void ChangeSceneWithTransition(const std::string& sceneName);

	/// <summary>
	/// 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ荳ｭ縺九←縺・°
	/// </summary>
	bool IsTransitioning();

	/// <summary>
	/// 蜑阪・繝輔Ξ繝ｼ繝縺九ｉ縺ｮ邨碁℃譎る俣・育ｧ抵ｼ・
	/// 騾壼ｸｸ縲・0繝輔Ξ繝ｼ繝縺ｮ蝣ｴ蜷医・ 0.01667 縺ｫ縺ｪ繧九′縲∝・逅・誠縺｡遲峨〒2繝輔Ξ繝ｼ繝縺九°繧後・ 0.0333 縺ｫ縺ｪ繧九・
	/// 譛蛻昴・10繝輔Ξ繝ｼ繝遞句ｺｦ縺ｯ蝓ｺ貅悶→縺ｪ繧区凾髢楢ｨ育ｮ励ｒ陦後▲縺ｦ縺・ｋ縺溘ａ縲∝ｮ牙ｮ壹☆繧九∪縺ｧ縺ｯ蛟､縺御ｸ崎ｦ丞援縺ｫ縺ｪ繧九・
	/// </summary>
	/// <returns>邨碁℃譎る俣・育ｧ抵ｼ・/returns>
	float DeltaTime();
    
	void Exit();

	void DebugInput();
	
	void DebugSceneInit();
};
