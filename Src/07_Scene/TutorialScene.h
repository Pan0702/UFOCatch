#pragma once

class CTutorialScene
{
public:
    CTutorialScene();
private:
    ~CTutorialScene();
    
    void ChangeStatate(const char* tutorialName);
    enum TutorialState
    {
        eMove = 0,
        eSuction,
        eDiscovery,
        eTimeUp
    };
};
