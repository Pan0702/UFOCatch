#pragma once
#include <string>
constexpr float NEAR_ZERO_LENSQ = 0.0001f;

///<summary>定数をまとめたnamespace</summary>
namespace Constants
{
    namespace FileName
    {
        constexpr const char* MODEL_LIST = "data/modelList.json";
        constexpr const char* UI_SPRITE = "data/PlayUIParts.png";
        constexpr const char* SELECT_BACK_IMAGE = "data/Select/_0006_Base2.png";
        constexpr const char* TITLE_BACK_IMAGE = "data/Title/TitleBack.jpg";
    }

    namespace SpriteName
    {
        constexpr const char* UI_SPRITE = "UiSprite";
        constexpr const char* GROUND = "Ground";
    }

    namespace SceneName
    {
        constexpr const char* TITLE = "TitleScene";
        constexpr const char* SELECT = "SelectScene";
        constexpr const char* EASY = "Easy";
        constexpr const char* NORMAL = "Normal";
        constexpr const char* PLAY = "Play";
        constexpr const char* RESULT = "ResultScene";
        constexpr const char* TUTORIAL = "Tutorial";
        constexpr const char* OI = "OI";
        constexpr const char* EDITOR = "Editor";
        constexpr const char* DEBUG = "Debug";
    }

    namespace Sound
    {
        namespace Path
        {
            constexpr const char* PLAY_BGM = "data/Sound/yukai.wav";
            constexpr const char* TITLE_BGM = "data/Sound/Sunny_day.wav";
            constexpr const char* SELECT_BGM = "data/Sound/himitu.wav";
            constexpr const char* SELECT_SE = "data/Sound/select_002.wav";
            constexpr const char* DECIDE_SE = "data/Sound/decide.wav";
        }

        namespace Key
        {
            constexpr const char* TITLE_BGM = "TITLE_BGM";
            constexpr const char* PLAY_BGM = "PLAY_BGM";
            constexpr const char* SELECT_BGM = "SELECT_BGM";
            constexpr const char* SELECT_SE = "SELECT_SE";
            constexpr const char* DECIDE_SE = "DECIDE_SE";
        }
    }

    namespace Model
    {
        constexpr const char* BACK_DROP = "data/Ground/CubeBoxSky.mesh";
        constexpr const char* GROUND = "data/Ground/Prefabs/MapPlane001.mesh";
    }

    namespace Animal
    {
        namespace Path
        {
            constexpr const char* DOG = "data/NewAnimal/Dog/Dog.mesh";
            constexpr const char* CHICKEN = "data/NewAnimal/Chicken/Chicken.mesh";
            constexpr const char* HUMAN = "data/NewAnimal/Human/Human.mesh";
        }

        namespace Name
        {
            const std::string DOG = "Dog";
            const std::string CHICKEN = "Chicken";
            const std::string HUMAN = "Human";
        }
    }
}

using namespace Constants;
