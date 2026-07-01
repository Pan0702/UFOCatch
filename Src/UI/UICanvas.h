#pragma once
#include "UIWidget.h"

/// @details ウィジェットはlayer順にソートして描画される
class CUICanvas
{
public:
    /// CUICanvas を初期化する
    /// @param size サイズ
    CUICanvas(const VECTOR2& size = VECTOR2(1366, 768));
    /// CUICanvas の終了処理を行う
    ~CUICanvas();

    /// @brief ウィジェットを追加して生ポインタを返す
    /// @details 所有権はキャンバスに移る。返されたポインタは所有しない参照として使う。
    template <typename T>
    /// Widget を追加する
    /// @param widget widget に渡す値
    /// @return 対象のポインタ
    T* AddWidget(std::unique_ptr<T> widget)
    {
        T* ptr = widget.get();
        m_widgets.push_back(std::move(widget));
        return ptr;
    }

    /// Widget を削除する
    /// @param widget widget に渡す値
    void RemoveWidget(CUIWidget* widget);

    /// 毎フレームの状態を更新する
    void Update();
    /// 描画する
    void Draw();

    /// クリアする
    void Clear();
    /// Canvas Size を取得する
    /// @return 2次元ベクトル
    VECTOR2 GetCanvasSize() const;

private:
    std::vector<std::unique_ptr<CUIWidget>> m_widgets;
    VECTOR2 m_canvasSize;
};
