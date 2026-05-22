#pragma once
/// <summary>
/// すべてのオブジェクトを管理します
/// シングルトンにしている
/// ここからすべてのGameObjectのUpdate()/Draw()を呼び出します
/// </summary>
/// <author>N.Hanai</author>

#include <list>
#include <memory>
#include <string>

/// <summary>ゲーム共通基盤で使う Quadtree System の情報と処理をまとめる型</summary>
class CQuadtreeSystem;
/// <summary>ObjectManagerに登録される全ゲームオブジェクトの基底クラス</summary>
class GameObject;

namespace ObjectManager
{
    /// 開始する
    void Start();
    /// 毎フレームの状態を更新する
    void Update();
    /// 描画する
    void Draw();
    /// 解放する
    void Release();
    /// Scene を切り替える
    void ChangeScene();

    /// 追加する
    /// @param obj 対象オブジェクト
    void Push(std::unique_ptr<GameObject> obj);

    /// 破棄する
    /// @param obj 対象オブジェクト
    void Destroy(GameObject* obj);

    /// All Objects を取得する
    /// @return 取得した要素一覧
    std::list<GameObject*> GetAllObjects();

    /// All Quad Tree を取得する
    /// @return 取得した要素一覧
    std::list<CQuadtreeSystem*> GetAllQuadTree();
    /// Tree を追加する
    /// @param tree tree に渡す値
    void PushTree(std::unique_ptr<CQuadtreeSystem> tree);

    /// Game Object を検索する
    /// @return 対象のポインタ
    template <class C>
    C* FindGameObject()
    {
        const std::list<GameObject*> objs = GetAllObjects();

        for (GameObject* node : objs)
        {
            C* obj = dynamic_cast<C*>(node);
            if (obj != nullptr)
                return obj;
        }
        return nullptr;
    }

    /// Game Objects を検索する
    /// @return 取得した要素一覧
    template <class C>
    std::list<C*> FindGameObjects()
    {
        std::list<C*> out;
        out.clear();

        const std::list<GameObject*> objs = GetAllObjects();

        for (GameObject* node : objs)
        {
            C* obj = dynamic_cast<C*>(node);
            if (obj != nullptr)
                out.emplace_back(obj);
        }
        return out;
    }

    /// Game Object With Tag を検索する
    /// @param tag タグ
    /// @return 対象のポインタ
    template <class C>
    C* FindGameObjectWithTag(std::string tag)
    {
        const std::list<GameObject*> objs = GetAllObjects();

        for (GameObject* node : objs)
        {
            C* obj = dynamic_cast<C*>(node);
            if (obj != nullptr)
            {
                if (obj->IsTag(tag))
                    return obj;
            }
        }
        return nullptr;
    }

    /// Game Objects With Tag を検索する
    /// @param tag タグ
    /// @return 取得した要素一覧
    template <class C>
    std::list<C*> FindGameObjectsWithTag(std::string tag)
    {
        std::list<C*> out;
        out.clear();

        const std::list<GameObject*> objs = GetAllObjects();

        for (GameObject* node : objs)
        {
            C* obj = dynamic_cast<C*>(node);
            if (obj != nullptr)
            {
                if (obj->IsTag(tag))
                    out.emplace_back(obj);
            }
        }
        return out;
    }

    /// Quad Tree を検索する
    /// @return 対象のポインタ
    template <class C>
    C* FindQuadTree()
    {
        const std::list<CQuadtreeSystem*> objs = GetAllQuadTree();

        for (CQuadtreeSystem* node : objs)
        {
            C* obj = dynamic_cast<C*>(node);
            if (obj != nullptr)
                return obj;
        }
        return nullptr;
    }

    /// Draw Order を設定する
    /// @param obj 対象オブジェクト
    /// @param _order _order に渡す値
    void SetDrawOrder(const GameObject* obj, int _order);

    /// Priority を設定する
    /// @param obj 対象オブジェクト
    /// @param _priority _priority に渡す値
    void SetPriority(const GameObject* obj, int _priority);

    /// Game Object を削除する
    /// @param obj 対象オブジェクト
    void DeleteGameObject(GameObject* obj);

    /// All Game Object を削除する
    void DeleteAllGameObject();

    /// Dont Destroy の処理を行う
    /// @param obj 対象オブジェクト
    /// @param dont dont に渡す値
    void DontDestroy(const GameObject* obj, bool dont = true);

    /// Active を設定する
    /// @param obj 対象オブジェクト
    /// @param active 有効フラグ
    void SetActive(const GameObject* obj, bool active = true);

    /// Visible を設定する
    /// @param obj 対象オブジェクト
    /// @param visible 表示フラグ
    void SetVisible(const GameObject* obj, bool visible = true);

    /// Exist を判定する
    /// @param obj 対象オブジェクト
    /// @return 成功または条件を満たす場合 true
    bool IsExist(GameObject* obj);


    /// All Quad Tree を削除する
    void DeleteAllQuadTree();
};
