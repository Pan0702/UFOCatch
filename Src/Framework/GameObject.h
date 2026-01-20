#pragma once
/// <summary>
/// ゲームオブジェクトの基底クラス
/// </summary>
/// <author>N.Hanai</author>

#include <string>
#include "SceneBase.h"
#include "ObjectManager.h"
#include "../03_GameMain/GameMain.h"

class GameObject {
public:
	GameObject() : pParent(nullptr), tag("") { ObjectManager::Push(this); }
	GameObject(GameObject* object) : pParent(object), tag("") { ObjectManager::Push(this); }
	virtual ~GameObject() {}

	/// <summary>
	/// オブジェクトを生成し、最初のUpdate()の前に呼ばれます
	/// </summary>
	virtual void Start() {}

	/// <summary>
	/// 毎フレームの更新処理のために呼ばれます
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// 毎フレームの描画処理のために呼ばれます
	/// </summary>
	virtual void Draw() {}

	/// <summary>
	/// 自分のインスタンスを削除する時に呼び出してください
	/// 次のUpdateの前に削除されます
	/// </summary>
	virtual void DestroyMe() {
		ObjectManager::Destroy(this);
	}

	/// <summary>
	/// シーンが変わっても削除されないようにします
	/// </summary>
	void DontDestroyMe() {
		ObjectManager::DontDestroy(this);
	}

	/// <summary>
	/// 更新の優先度を指定します
	/// 値が大きいほど後に実行されます
	/// 何も指定しなければ、0です
	/// </summary>
	/// <param name="pri">優先度</param>
	void SetPriority(int pri) {
		ObjectManager::SetPriority(this, pri);
	}

	/// <summary>
	/// 描画の優先度を指定します
	/// 値が大きいほど、後から描画されるので、
	/// 2Dであれば、手前に表示されます
	/// 何も指定しなければ0です
	/// </summary>
	/// <param name="odr">描画順序</param>
	void SetDrawOrder(int odr) {
		ObjectManager::SetDrawOrder(this, odr);
	}

	/// <summary>
	/// タグを設定
	/// タグは1つだけ付けることができます
	/// </summary>
	/// <param name="_tag">タグ</param>
	void SetTag(std::string _tag) { tag = _tag; }

	/// <summary>
	/// 指定されたタグと同じか返す
	/// </summary>
	/// <param name="_tag">タグ</param>
	/// <returns>同じであればtrue</returns>
	bool IsTag(std::string _tag) const { return tag == _tag; }

	/// <summary>
	/// 親のオブジェクトを返す
	/// </summary>
	/// <returns>親オブジェクトのポインター</returns>
	GameObject* Parent() const { return pParent; }

	/// <summary>
	/// 親のオブジェクトを設定する
	/// </summary>
	/// <returns>親オブジェクトのポインター</returns>
	void SetParent(GameObject* _parent) { pParent = _parent; }

private:
	GameObject* pParent; // 親のオブジェクト
	std::string tag;     // タグ
};

template <class C> C* Instantiate()
{
	C* obj = new C;
	return obj;
};

template <class C> C* Instantiate(GameObject* parent)
{
	C* obj = new C(parent);
	return obj;
};

template <class C> C* SingleInstantiate()
{
	C* obj = ObjectManager::FindGameObject<C>();
	if (obj == nullptr) {
		obj = Instantiate<C>();
	}
	return obj;
}
