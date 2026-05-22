#pragma once
/// <summary>ゲームオブジェクトの生成ヘルパと基底処理を定義するヘッダー</summary>
/// <author>N.Hanai</author>

#include <string>
#include "SceneBase.h"
#include "ObjectManager.h"
#include "../Core/Game/GameMain.h"
#include "../Framework/AudioManager.h"

/// <summary>ObjectManagerに登録される全ゲームオブジェクトの基底クラス</summary>
class GameObject
{
public:
    /// GameObject を初期化する
    GameObject() : pParent(nullptr), tag("")
    {
    }


    /// GameObject の終了処理を行う
    virtual ~GameObject()
    {
    }

    /// 開始する
    virtual void Start()
    {
    }

    /// 毎フレームの状態を更新する
    virtual void Update()
    {
    }

    /// 描画する
    virtual void Draw()
    {
    }

    /// Me を破棄する
    virtual void DestroyMe()
    {
        ObjectManager::Destroy(this);
        AudioManager::Play(_T("SuctionEnd"), false);
    }

    /// Dont Destroy Me の処理を行う
    void DontDestroyMe()
    {
        ObjectManager::DontDestroy(this);
    }

    /// Priority を設定する
    /// @param pri pri に渡す値
    void SetPriority(int pri)
    {
        ObjectManager::SetPriority(this, pri);
    }

    /// Draw Order を設定する
    /// @param odr odr に渡す値
    void SetDrawOrder(int odr)
    {
        ObjectManager::SetDrawOrder(this, odr);
    }

    /// Tag を設定する
    /// @param _tag タグ
    void SetTag(std::string _tag) { tag = _tag; }

    /// Tag を判定する
    /// @param _tag タグ
    /// @return 成功または条件を満たす場合 true
    bool IsTag(std::string _tag) const { return tag == _tag; }

    /// Parent を返す
    /// @return 対象のポインタ
    GameObject* Parent() const { return pParent; }

    /// Parent を設定する
    /// @param _parent 親オブジェクト
    void SetParent(GameObject* _parent) { pParent = _parent; }

private:
    GameObject* pParent; // 親のオブジェクト
    std::string tag; // タグ
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
/// Instantiate を返す
/// @return 対象のポインタ
C* Instantiate()
{
    auto obj = std::make_unique<C>();
    C* raw = obj.get();
    ObjectManager::Push(std::move(obj));
    return raw;
}

template <class C>
/// Instantiate を返す
/// @param parent 親オブジェクト
/// @return 対象のポインタ
C* Instantiate(GameObject* parent)
{
    auto obj = std::make_unique<C>(parent);
    C* raw = obj.get();
    ObjectManager::Push(std::move(obj));
    return raw;
}

template <class C, class... Args>
/// Instantiate を返す
/// @param args args に渡す値
/// @return 対象のポインタ
C* Instantiate(Args&&... args)
{
    auto obj = std::make_unique<C>(std::forward<Args>(args)...);
    C* raw = obj.get();
    ObjectManager::Push(std::unique_ptr<GameObject>(std::move(obj)));
    return raw;
}

template <class C>
/// Single Instantiate を返す
/// @return 対象のポインタ
C* SingleInstantiate()
{
    C* obj = ObjectManager::FindGameObject<C>();
    if (obj == nullptr)
    {
        obj = Instantiate<C>();
    }
    return obj;
}
