#pragma once
#include "../../Framework/SceneBase.h"
#include "../../Utils/WipeAnimator.h"

class CSelectionScene : public SceneBase
{
public:
    
    CSelectionScene();
    ~CSelectionScene();

private:
    void Update() override;
    void Draw() override;
    void InitButtons();
    void InitScene();
    void PlayButton();
    void LevelButton();
    void ButtonsDraw();
    void InitImage();
    //Buttonに関する情報
    struct ButtomInfo
    {
        CSpriteImage* image;
        VECTOR2 imageSize;
        std::string sceneName;
    };

    std::vector<ButtomInfo> m_buttons;
    std::vector<std::string> m_sceneName;
    std::vector<CSpriteImage*> m_images;
    CSpriteImage* m_pImageBackGround;
    int m_selectedIndex;
    int m_play = 0;
    WipeAnimator m_wipeAnim{ 3 };
};

