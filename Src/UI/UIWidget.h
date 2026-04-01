#pragma once
#include <cstdint>
#include "../Utils/Sprite3D.h"

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

///全てのUI要素の基底クラス。階層構造、位置管理、描画の基本機能を提供。///
class UIWidget
{
public:
    UIWidget();
    template<typename T>
    T* AddChild(std::unique_ptr<T> child);
    void RemoveChild(UIWidget* child);
    void ClearChild();
    
    virtual void Update();
    virtual void Draw(CSprite& sprite);
    
    void SetAnchor(AnchorType anchor);
    void SetPosition(const VECTOR2& pos);
    void SetSize(const VECTOR2& size);
    void SetLayer(int layer);
    void SetAlpha(float alpha);
    void SetVisible(bool visible);
    
    VECTOR2 GEtWorldPosition();
    const VECTOR2& GetPosition();
    const VECTOR2& GetSize();
    int GetLayer() const;
    float GetAlpha() const;
    bool IsVisible() const;
protected:
    VECTOR2 CalcAnchorOffset();
protected:
    VECTOR2 m_position = {};        //親からの相対位置
    VECTOR2 m_size = {};            //サイズ
    int m_layer = 0;                //アンカー
    float m_alpha = 0.0f;           //描画レイヤー
    AnchorType m_anchor;            //透明度
    bool m_visible = false;         //表示フラグ
    
    UIWidget* m_pParent = nullptr;  //親ウィジェット(所有しない)
    std::vector<std::unique_ptr<UIWidget>> m_children;//子Widget
};
