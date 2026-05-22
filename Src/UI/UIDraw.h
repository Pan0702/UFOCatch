#pragma once
#include "../Utils/Sprite3D.h"

/// @brief CUIDrawが管理する1つの描画要素
struct UIImageInfo
{
    CSpriteImage* pTexture; // ImageRegistryから取得したポインタ（所有しない）
    VECTOR2 position;
    VECTOR4 imageSize; // (srcX, srcY, srcWidth, srcHeight)
    float alpha;
    int layer;

    /// UIImageInfo を初期化する
    /// @param tex 使用するテクスチャ
    /// @param pos 座標
    /// @param size サイズ
    /// @param a a に渡す値
    /// @param l l に渡す値
    UIImageInfo(CSpriteImage* tex, const VECTOR2& pos, const VECTOR4&
                size, float a = 1.0f, int l = 0)
        : pTexture(tex), position(pos),
          imageSize(size), alpha(a), layer(l)
    {
    }

    /// UIImageInfo を初期化する
    UIImageInfo() = default;
};

/// @brief ウィジェット階層を使わずに画像をシンプルに描画する軽量UIクラス
/// @details AddElement()で要素を登録し、Draw()でlayer順に一括描画する。
class CUIDraw
{
public:
    /// Element を追加する
    /// @param name 名前
    /// @param pos 座標
    /// @param size サイズ
    /// @param alpha 透明度
    /// @param layer layer に渡す値
    /// @return 処理結果の数値
    int AddElement(const char* name, const VECTOR2& pos, const VECTOR4& size, float alpha = 1.0f, int layer = 0);
    /// クリアする
    void Clear();
    /// Pos を設定する
    /// @param index インデックス
    /// @param pos 座標
    void SetPos(int index, const VECTOR2& pos);
    /// Alpha を設定する
    /// @param index インデックス
    /// @param alpha 透明度
    void SetAlpha(int index, float alpha);
    /// Layer を設定する
    /// @param index インデックス
    /// @param layer layer に渡す値
    void SetLayer(int index, int layer);
    /// 描画する
    void Draw();
    /// Element Count を取得する
    /// @return 処理結果の数値
    int GetElementCount();
    /// Element を取得する
    /// @param index インデックス
    /// @return 処理結果
    UIImageInfo GetElement(int index) const;

private:
    std::vector<UIImageInfo> m_elements;
};
