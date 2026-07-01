#pragma once
#include <vector>

template <typename T>
/// <summary>エンジン基盤で使う Quad Tree Cell の情報と処理をまとめる型</summary>
class CQuadTreeCell
{
public:
    /// 追加する
    /// @param object 対象オブジェクト
    void Push(T* object)
    {
        m_objects.push_back(object);
    }

    /// クリアする
    void Clear()
    {
        m_objects.clear();
    }

    /// Objects を取得する
    /// @return 取得した要素一覧
    const std::vector<T*>& GetObjects()
    {
        return m_objects;
    }

private:
    std::vector<T*> m_objects;
};
