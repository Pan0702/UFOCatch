#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>

#include "../System/GameInstance.h"
#include "../Scene/Title/TitleScene.h"
#include "../Scene/Play/PlayScene.h"
#include "../Scene/Result/ResultScene.h"
#include "../Scene/Select/SelectionScene.h"
#include "../Scene/Play/TutorialScene.h"
#include "../Enemies/AnimalChicken/Chicken.h"
#include "../Scene/Other/DebugScene.h"
#include "../Scene/Other/OIScene.h"
#include "../Scene/Play/NormalScene.h"
#include "../Scene/Editor/EditorScene.h"
#include "../Enemies/System/ModelRegistry.h"

using namespace Constants::SceneName;
std::unique_ptr<SceneBase> SceneFactory::CreateFirst()
{
    SingleInstantiate<CGameInstance>();
    SingleInstantiate<CModelRegistry>();
    return std::make_unique<TitleScene>();

    return nullptr;
}

std::unique_ptr<SceneBase> SceneFactory::Create(const std::string& name)
{
    if (name == TITLE)
    {
        return std::make_unique<TitleScene>();
    }
    if (name == SELECT)
    {
        return std::make_unique<CSelectionScene>();
    }
    if (name == EASY)
    {
        return std::make_unique<PlayScene>();
    }
    if (name == NORMAL)
    {
        return std::make_unique<CNormalScene>();
    }
    if (name == EDITOR)
    {
        return std::make_unique<CEditorScene>();
    }
    if (name == RESULT)
    {
        return std::make_unique<CResultScene>();
    }
    if (name == TUTORIAL)
    {
        return std::make_unique<CTutorialScene>();
    }
    if (name == DEBUG)
    {
        return std::make_unique<CDebugScene>();
    }
    if (name == OI)
    {
        return std::make_unique<OIScene>();
    }
    //
    assert(false);
    return nullptr;
}
