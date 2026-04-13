#pragma once
#include <cstdint>

#include "UIAnimationPlayer.h"
#include "../Utils/Sprite3D.h"

/// @brief ウィジェットの基準点（親の矩形内のどこを原点とするか）
enum class AnchorType : uint8_t
{
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
};

/// @brief 全UIウィジェットの基底クラス
/// @details 親子階層・位置・サイズ・描画レイヤー・アニメーションの管理を行う。
///          子ウィジェットはlayer順にソートして描画される。
class CUIWidget
{
public:
    CUIWidget();

    /// @brief 子ウィジェットを追加して生ポインタを返す
    /// @details 所有権はこのウィジェットに移る。返されたポインタは所有しない参照として使う。
    template<typename T>
    T* AddChild(std::unique_ptr<T> child)
    {
        T* ptr = child.get();
        ptr->m_pParent = this;
        m_children.push_back(std::move(child));
        return ptr;
    }

    /// @brief 指定した子ウィジェットを削除する
    void RemoveChild(CUIWidget* child);
    void ClearChild();

    virtual void Update();
    virtual void Draw(CSprite& sprite);

    void SetAnchor(AnchorType anchor);
    void SetPosition(const VECTOR2& pos);
    void SetSize(const VECTOR2& size);
    void SetLayer(int layer);
    void SetAlpha(float alpha);
    void SetVisible(bool visible);

    /// @brief アンカーと親位置を考慮したワールド座標を返す
    VECTOR2 GetWorldPosition() const;
    const VECTOR2& GetPosition();
    const VECTOR2& GetSize();
    int GetLayer() const;
    float GetAlpha() const;
    bool IsVisible() const;
    CUIAnimationPlayer& GetAnimationPlayer();

protected:
    /// @brief アンカータイプに応じた親基準のオフセットを計算する
    VECTOR2 CalcAnchorOffset() const;

    /// @brief アニメーションプレイヤーの現在値をウィジェットのプロパティに反映する
    virtual void ApplyAnimValues();

protected:
    VECTOR2 m_position = {};         // 親からの相対位置
    VECTOR2 m_size = {};             // サイズ
    int m_layer = 0;                 // 描画レイヤー（値が小さいほど手前に描画）
    float m_alpha = 1.0f;            // 透明度（0.0〜1.0）
    AnchorType m_anchor;             // アンカータイプ
    bool m_visible = true;           // 表示フラグ

    CUIWidget* m_pParent = nullptr;                      // 親ウィジェット（所有しない）
    std::vector<std::unique_ptr<CUIWidget>> m_children;  // 子ウィジェット一覧
    CUIAnimationPlayer m_pAnimPlayer;
    VECTOR2 m_scale = VECTOR2(1, 1);                     // スケール（アニメーションから反映）

};
