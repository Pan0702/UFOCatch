#pragma once
/// <summary>
/// 繧ｲ繝ｼ繝繧ｪ繝悶ず繧ｧ繧ｯ繝医・蝓ｺ蠎輔け繝ｩ繧ｹ
/// </summary>
/// <author>N.Hanai</author>

#include <string>
#include "SceneBase.h"
#include "ObjectManager.h"
#include "../Core/Game/GameMain.h"
#include "../Framework/AudioManager.h"

class GameObject
{
public:
    GameObject() : pParent(nullptr), tag("")
    {
    }



    virtual ~GameObject()
    {
    }

    /// <summary>
    /// 繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ逕滓・縺励∵怙蛻昴・Update()縺ｮ蜑阪↓蜻ｼ縺ｰ繧後∪縺・
    /// </summary>
    virtual void Start()
    {
    }

    /// <summary>
    /// 豈弱ヵ繝ｬ繝ｼ繝縺ｮ譖ｴ譁ｰ蜃ｦ逅・・縺溘ａ縺ｫ蜻ｼ縺ｰ繧後∪縺・
    /// </summary>
    virtual void Update()
    {
    }

    /// <summary>
    /// 豈弱ヵ繝ｬ繝ｼ繝縺ｮ謠冗判蜃ｦ逅・・縺溘ａ縺ｫ蜻ｼ縺ｰ繧後∪縺・
    /// </summary>
    virtual void Draw()
    {
    }

    /// <summary>
    /// 閾ｪ蛻・・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧貞炎髯､縺吶ｋ譎ゅ↓蜻ｼ縺ｳ蜃ｺ縺励※縺上□縺輔＞
    /// 谺｡縺ｮUpdate縺ｮ蜑阪↓蜑企勁縺輔ｌ縺ｾ縺・
    /// </summary>
    virtual void DestroyMe()
    {
        ObjectManager::Destroy(this);
        AudioManager::Play(_T("SuctionEnd"), false);
    }

    /// <summary>
    /// 繧ｷ繝ｼ繝ｳ縺悟､峨ｏ縺｣縺ｦ繧ょ炎髯､縺輔ｌ縺ｪ縺・ｈ縺・↓縺励∪縺・
    /// </summary>
    void DontDestroyMe()
    {
        ObjectManager::DontDestroy(this);
    }

    /// <summary>
    /// 譖ｴ譁ｰ縺ｮ蜆ｪ蜈亥ｺｦ繧呈欠螳壹＠縺ｾ縺・
    /// 蛟､縺悟､ｧ縺阪＞縺ｻ縺ｩ蠕後↓螳溯｡後＆繧後∪縺・
    /// 菴輔ｂ謖・ｮ壹＠縺ｪ縺代ｌ縺ｰ縲・縺ｧ縺・
    /// </summary>
    /// <param name="pri">蜆ｪ蜈亥ｺｦ</param>
    void SetPriority(int pri)
    {
        ObjectManager::SetPriority(this, pri);
    }

    /// <summary>
    /// 謠冗判縺ｮ蜆ｪ蜈亥ｺｦ繧呈欠螳壹＠縺ｾ縺・
    /// 蛟､縺悟､ｧ縺阪＞縺ｻ縺ｩ縲∝ｾ後°繧画緒逕ｻ縺輔ｌ繧九・縺ｧ縲・
    /// 2D縺ｧ縺ゅｌ縺ｰ縲∵焔蜑阪↓陦ｨ遉ｺ縺輔ｌ縺ｾ縺・
    /// 菴輔ｂ謖・ｮ壹＠縺ｪ縺代ｌ縺ｰ0縺ｧ縺・
    /// </summary>
    /// <param name="odr">謠冗判鬆・ｺ・/param>
    void SetDrawOrder(int odr)
    {
        ObjectManager::SetDrawOrder(this, odr);
    }

    /// <summary>
    /// 繧ｿ繧ｰ繧定ｨｭ螳・
    /// 繧ｿ繧ｰ縺ｯ1縺､縺縺台ｻ倥￠繧九％縺ｨ縺後〒縺阪∪縺・
    /// </summary>
    /// <param name="_tag">繧ｿ繧ｰ</param>
    void SetTag(std::string _tag) { tag = _tag; }

    /// <summary>
    /// 謖・ｮ壹＆繧後◆繧ｿ繧ｰ縺ｨ蜷後§縺玖ｿ斐☆
    /// </summary>
    /// <param name="_tag">繧ｿ繧ｰ</param>
    /// <returns>蜷後§縺ｧ縺ゅｌ縺ｰtrue</returns>
    bool IsTag(std::string _tag) const { return tag == _tag; }

    /// <summary>
    /// 隕ｪ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑斐☆
    /// </summary>
    /// <returns>隕ｪ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ繝ｼ</returns>
    GameObject* Parent() const { return pParent; }

    /// <summary>
    /// 隕ｪ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ險ｭ螳壹☆繧・
    /// </summary>
    /// <returns>隕ｪ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ繝ｼ</returns>
    void SetParent(GameObject* _parent) { pParent = _parent; }

private:
    GameObject* pParent; // 隕ｪ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝・
    std::string tag; // 繧ｿ繧ｰ
};

// template <class C> C* Instantiate()
// {
// 	auto obj = std::make_unique<C>();                        
// 	C* raw = obj.get();
// 	ObjectManager::Push(std::move(obj));
// 	return raw;
// };
//
// template <class C> C* Instantiate(GameObject* parent)
// {
// 	auto obj = std::make_unique<C>(parent);                                  
// 	C* raw = obj.get();
// 	ObjectManager::Push(std::move(obj));
// 	return raw;
// };

template <class C>
 C* Instantiate()
{
    auto obj = std::make_unique<C>();
    C* raw = obj.get();
    ObjectManager::Push(std::move(obj));
    return raw;
}

template <class C>
 C* Instantiate(GameObject* parent)
{
    auto obj = std::make_unique<C>(parent);
    C* raw = obj.get();
    ObjectManager::Push(std::move(obj));
    return raw;
}

template <class C, class... Args>
 C* Instantiate(Args&&... args)
{
    auto obj = std::make_unique<C>(std::forward<Args>(args)...);
    C* raw = obj.get();
    ObjectManager::Push(std::unique_ptr<GameObject>(std::move(obj)));  
    return raw;
}
template <class C>
C* SingleInstantiate()
{
    C* obj = ObjectManager::FindGameObject<C>();
    if (obj == nullptr)
    {
        obj = Instantiate<C>();
    }
    return obj;
}

