#pragma once
/// <summary>
/// すべてのオブジェクトを管理します
/// シングルトンにしている
/// ここからすべてのGameObjectのUpdate()/Draw()を呼び出します
/// </summary>
/// <author>N.Hanai</author>

#include <list>
#include <string>
class GameObject;

namespace ObjectManager {
	void Start();
	void Update();
	void Draw();
	void Release();
	void ChangeScene();

	/// <summary>
	/// Objectを追加する
	/// この関数は、GameObjectのコンストラクタから呼ばれる
	/// </summary>
	/// <param name="obj"></param>
	void Push(GameObject* obj);

	/// <summary>
	/// Objectを削除する
	/// この関数では、削除の要求をするだけで、
	/// 実際に削除されるのは、Update()が呼ばれる直前
	/// </summary>
	/// <param name="obj"></param>
	void Destroy(GameObject* obj);

	std::list<GameObject*> GetAllObjects();

	/// <summary>
	/// クラス名でオブジェクトを探す
	/// </summary>
	/// <typeparam name="C">クラス</typeparam>
	/// <returns>オブジェクトの実態（存在しなければnullptr）</returns>
	template<class C> C* FindGameObject()
	{
		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr)
				return obj;
		}
		return nullptr;
	}

	/// <summary>
	/// クラスのオブジェクトをすべて探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <returns>オブジェクトの実態list</returns>
	template<class C> std::list<C*> FindGameObjects()
	{
		std::list<C*> out;
		out.clear();

		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr)
				out.emplace_back(obj);
		}
		return out;
	}

	/// <summary>
	/// クラス名とタグからオブジェクトを探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <param name="tag">タグ</param>
	/// <returns>オブジェクトの実態（存在しなければnullptr）</returns>
	template<class C> C* FindGameObjectWithTag(std::string tag)
	{
		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr) {
				if (obj->IsTag(tag))
					return obj;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// クラス名とタグからオブジェクトをすべて探す
	/// </summary>
	/// <typeparam name="C">クラス名</typeparam>
	/// <param name="tag">タグ</param>
	/// <returns>オブジェクトの実態list</returns>
	template<class C> std::list<C*> FindGameObjectsWithTag(std::string tag)
	{
		std::list<C*> out;
		out.clear();

		const std::list<GameObject*> objs = GetAllObjects();

		for (GameObject* node : objs) {
			C* obj = dynamic_cast<C*>(node);
			if (obj != nullptr) {
				if (obj->IsTag(tag))
					out.emplace_back(obj);
			}
		}
		return out;
	}

	/// <summary>
	/// 描画のプライオリティを設定する
	/// 数値が少ない順に描画されるので、２Ｄでは奥に表示される
	/// ２Ｄで手前に表示したい時、３Ｄで後に描画したい時は、値を高くする
	/// プライオリティが同じものの順番は保証されない
	/// プライオリティのデフォルトは０です
	/// </summary>
	/// <param name="obj">プライオリティを設定するオブジェクト</param>
	/// <param name="order">描画プライオリティ</param>
	void SetDrawOrder(GameObject* _obj, int _order);

	/// <summary>
	/// Updateの優先順位を付ける
	/// </summary>
	/// <param name="_obj"></param>
	/// <param name="_priority"></param>
	void SetPriority(GameObject* _obj, int _priority);

	/// <summary>
	/// GameObjectを削除する
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	void DeleteGameObject(GameObject* obj);

	/// <summary>
	/// 全てのGameObjectを削除する
	/// </summary>
	void DeleteAllGameObject();

	void DontDestroy(GameObject* obj, bool dont = true);

	/// <summary>
	/// Updateを実行するか設定する
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	/// <param name="active">実行する場合はtrue</param>
	void SetActive(GameObject* obj, bool active = true);

	/// <summary>
	/// Drawを実行するか設定する
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	/// <param name="visible">Drawする場合はtrue</param>
	void SetVisible(GameObject* obj, bool visible = true);

	/// <summary>
	/// 指定のオブジェクトが存在するかを調べる
	/// Activeでも、非Activeでも、存在していればtrueとなる
	/// </summary>
	/// <param name="obj">GameObjectのインスタンス</param>
	/// <returns>存在すれtrue</returns>
	bool IsExist(GameObject* obj);
};
