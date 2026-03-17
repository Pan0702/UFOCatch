#include <algorithm>
#include <cassert>

#include "sceneManager.h"
#include "sceneBase.h"
#include "SceneFactory.h"
#include "../Utils/ScreenTransition.h"
#include <Windows.h>

namespace
{
    std::string currentName;
    std::string nextName;
    std::unique_ptr<SceneBase> currentScene;
    std::unique_ptr<SceneFactory> factory;

    // 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ
    std::unique_ptr<CScreenTransition> transition;
    std::string pendingSceneName; // 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ荳ｭ縺ｮ谺｡縺ｮ繧ｷ繝ｼ繝ｳ蜷・
    bool waitingForFadeIn = false; // FadeIn蠕・■迥ｶ諷・

    // DeltaTime逕ｨ
    LARGE_INTEGER freq;
    LARGE_INTEGER current;
    float deltaTime;
     constexpr int REC_SIZE = 60;
    float record[REC_SIZE];
    int recCount = 0;

    void timeInit()
    {
        bool ret = QueryPerformanceFrequency(&freq);
        assert(ret);
        QueryPerformanceCounter(&current);
    }

    void timeUpdate()
    {
        LARGE_INTEGER last = current;
        QueryPerformanceCounter(&current);
        float t = static_cast<float>(current.QuadPart - last.QuadPart) / freq.QuadPart;
        float t2 = t;
        // deltaTime縺ｯ縲∝ｹｳ蝮・ヵ繝ｬ繝ｼ繝繝ｬ繝ｼ繝医・2蛟阪ｒ雜・∴縺ｪ縺・ｈ縺・↓縺吶ｋ
        if (recCount >= REC_SIZE)
        {
            float sum = 0;
            for (int i = 0; i < REC_SIZE; i++)
            {
                sum += record[i];
            }
            sum /= REC_SIZE;
            t2 = (std::min)(t2, sum * 3.0f);
        }
        record[recCount % REC_SIZE] = t;
        recCount++;
        deltaTime = t2;
    }
};

void SceneManager::Start()
{
    timeInit();
    nextName = "";
    currentName = "";

    factory =  std::make_unique<SceneFactory>();
    transition = std::make_unique<CScreenTransition>();
    currentScene = factory->CreateFirst();
}

void SceneManager::Update()
{
    timeUpdate();

    // 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ譖ｴ譁ｰ
    if (transition)
        transition->Update(deltaTime);

    if (nextName != currentName)
    {
        if (currentScene != nullptr)
        {
            currentScene.reset();
        }
        currentName = nextName;
        currentScene = factory->Create(nextName);
        // 繧ｷ繝ｼ繝ｳ逕滓・縺ｯ繝ｭ繝ｼ繝牙・逅・〒譎る俣縺後°縺九ｋ蝣ｴ蜷医′縺ゅｋ縲・
        // 繝ｭ繝ｼ繝画凾髢薙ｒ谺｡繝輔Ξ繝ｼ繝縺ｮ deltaTime 縺ｫ蜷ｫ繧√↑縺・ｈ縺・ち繧､繝繧ｹ繧ｿ繝ｳ繝励ｒ繝ｪ繧ｻ繝・ヨ縺吶ｋ縲・
        QueryPerformanceCounter(&current);
    }

    // 繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺亥ｾ後↓FadeIn髢句ｧ・
    if (waitingForFadeIn && nextName == currentName)
    {
        waitingForFadeIn = false;
        // 繧ｷ繝ｼ繝ｳ縺ｮ繝ｭ繝ｼ繝会ｼ・urrentScene縺ｮ逕滓・・峨′螳御ｺ・＠縺溽峩蠕後↓FadeIn繧帝幕蟋九☆繧・
        transition->StartFadeIn();
    }
    if (currentScene != nullptr)
        currentScene->Update();
}

void SceneManager::Draw()
{
    if (currentScene != nullptr)
        currentScene->Draw();
}

void SceneManager::DrawTransition()
{
    // 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ縺ｯ譛蜑埼擇縺ｫ謠冗判・・bjectManager::Draw()繧医ｊ蠕後↓蜻ｼ縺ｶ縺薙→・・
    if (transition)
        transition->Draw();
}

void SceneManager::Release()
{
    if (currentScene != nullptr)
    {
        currentScene.reset();
    }
    transition.reset();
    factory.reset();
}

SceneBase* SceneManager::CurrentScene()
{
    return currentScene.get();
}

static void SceneManager::SetCurrentScene(std::unique_ptr<SceneBase> scene)
{
    currentScene = std::move(scene);
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
    nextName = sceneName;
}

void SceneManager::ChangeSceneWithTransition(const std::string& sceneName)
{
    if (transition->IsTransitioning())
        return; // 繝医Λ繝ｳ繧ｸ繧ｷ繝ｧ繝ｳ荳ｭ縺ｯ辟｡隕・

    pendingSceneName = sceneName;
    transition->StartFadeOut([&]()
    {
        // FadeOut螳御ｺ・凾縺ｫ繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺・
        nextName = pendingSceneName;
        waitingForFadeIn = true;
    });
}

bool SceneManager::IsTransitioning()
{
    return transition && transition->IsTransitioning();
}

float SceneManager::DeltaTime()
{
    return deltaTime;
}


void SceneManager::Exit()
{
    PostQuitMessage(0);
}
