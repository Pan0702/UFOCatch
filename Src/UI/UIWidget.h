#pragma once
#include <cstdint>

#include "UIAnimationPlayer.h"
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
class CUIWidget
{
public:
    CUIWidget();
    template<typename T>
    T* AddChild(std::unique_ptr<T> child)
    {
        T* ptr = child.get();
        ptr->m_pParent = this;
        m_children.push_back(std::move(child));
        return ptr;
    }
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
    
    VECTOR2 GetWorldPosition();
    const VECTOR2& GetPosition();
    const VECTOR2& GetSize();
    int GetLayer() const;
    float GetAlpha() const;
    bool IsVisible() const;
    CUIAnimationPlayer& GetAnimationPlayer();
protected:
    VECTOR2 CalcAnchorOffset() const;
    virtual void ApplayAnimValues();
protected:
    VECTOR2 m_position = {};        //親からの相対位置
    VECTOR2 m_size = {};            //サイズ
    int m_layer = 0;                //アンカー
    float m_alpha = 0.0f;           //描画レイヤー
    AnchorType m_anchor;            //透明度
    bool m_visible = false;         //表示フラグ
    
    CUIWidget* m_pParent = nullptr;  //親ウィジェット(所有しない)
    std::vector<std::unique_ptr<CUIWidget>> m_children;//子Widget
    CUIAnimationPlayer m_pAnimPlayer;
    VECTOR2 m_scale = VECTOR2(1, 1);
    
};
