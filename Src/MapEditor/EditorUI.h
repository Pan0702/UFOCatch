#pragma once
#include "../Common/Object3D.h"

class Button;
class TRS;
class Controller;

class EditorUI : public Object3D
{
public:
    EditorUI();
    ~EditorUI() = default;

private:
    Button*     m_pButton;
    TRS*        m_pTRS;
    Controller* m_pController;
    
    void Draw() override;

    void DrawEditorToolsWindow() const;
    void DrawSettingWindow() const;
    void DrawHierarchyWindow() const;
    void DrawTransformWindow() const;
    void DrawMoveAmountWindow() const;
};