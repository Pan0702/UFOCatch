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
///          子ウィジェットはlayer順にソートして描画される。
class CUIWidget
{
public:
    /// CUIWidget を初期化する
    CUIWidget();

    /// @brief 子ウィジェットを追加して生ポインタを返す
    /// @details 所有権はこのウィジェットに移る。返されたポインタは所有しない参照として使う。
    template <typename T>
    /// Child を追加する
    /// @param child child に渡す値
    /// @return 対象のポインタ
    T* AddChild(std::unique_ptr<T> child)
    {
        T* ptr = child.get();
        ptr->m_pParent = this;
        m_children.push_back(std::move(child));
        return ptr;
    }

    /// Child を削除する
    /// @param child child に渡す値
    void RemoveChild(CUIWidget* child);
    /// Child をクリアする
    void ClearChild();

    /// 毎フレームの状態を更新する
    virtual void Update();
    /// 描画する
    /// @param sprite sprite に渡す値
    virtual void Draw(CSprite& sprite);

    /// Anchor を設定する
    /// @param anchor anchor に渡す値
    void SetAnchor(AnchorType anchor);
    /// Position を設定する
    /// @param pos 座標
    void SetPosition(const VECTOR2& pos);
    /// Size を設定する
    /// @param size サイズ
    void SetSize(const VECTOR2& size);
    /// Layer を設定する
    /// @param layer layer に渡す値
    void SetLayer(int layer);
    /// Alpha を設定する
    /// @param alpha 透明度
    void SetAlpha(float alpha);
    /// Visible を設定する
    /// @param visible 表示フラグ
    void SetVisible(bool visible);

    /// World Position を取得する
    /// @return 2次元ベクトル
    VECTOR2 GetWorldPosition() const;
    /// Position を取得する
    /// @return 2次元ベクトル
    const VECTOR2& GetPosition();
    /// Size を取得する
    /// @return 2次元ベクトル
    const VECTOR2& GetSize();
    /// Layer を取得する
    /// @return 処理結果の数値
    int GetLayer() const;
    /// Alpha を取得する
    /// @return 計算結果の値
    float GetAlpha() const;
    /// Visible を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsVisible() const;
    /// Animation Player を取得する
    /// @return 処理結果
    CUIAnimationPlayer& GetAnimationPlayer();

protected:
    /// Anchor Offset を計算する
    /// @return 2次元ベクトル
    VECTOR2 CalcAnchorOffset() const;

    /// Apply Anim Values の処理を行う
    virtual void ApplyAnimValues();

protected:
    VECTOR2 m_position = {}; // 親からの相対位置
    VECTOR2 m_size = {}; // サイズ
    int m_layer = 0; // 描画レイヤー（値が小さいほど手前に描画）
    float m_alpha = 1.0f; // 透明度（0.0〜1.0）
    AnchorType m_anchor; // アンカータイプ
    bool m_visible = true; // 表示フラグ

    CUIWidget* m_pParent = nullptr; // 親ウィジェット（所有しない）
    std::vector<std::unique_ptr<CUIWidget>> m_children; // 子ウィジェット一覧
    CUIAnimationPlayer m_pAnimPlayer;
    VECTOR2 m_scale = VECTOR2(1, 1); // スケール（アニメーションから反映）
};
