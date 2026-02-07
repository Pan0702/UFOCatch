#include <algorithm>
#include <cassert>

#include "sceneManager.h"
#include "sceneBase.h"
#include "SceneFactory.h"
#include "../Scene/ScreenTransition.h"
#include <Windows.h>

namespace
{
    std::string currentName;
    std::string nextName;
    SceneBase* currentScene;
    SceneFactory* factory;

    // トランジション
    std::unique_ptr<CScreenTransition> transition;
    std::string pendingSceneName; // トランジション中の次のシーン名
    bool waitingForFadeIn = false; // FadeIn待ち状態

    // DeltaTime用
    LARGE_INTEGER freq;
    LARGE_INTEGER current;
    float deltaTime;
    static constexpr int REC_SIZE = 60;
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
        // deltaTimeは、平均フレームレートの2倍を超えないようにする
        if (recCount >= REC_SIZE)
        {
            float sum = 0;
            for (int i = 0; i < REC_SIZE; i++)
                sum += record[i];
            sum /= REC_SIZE;
            t2 = (std::min)(t2, sum * 3.0f);
            //			if (t2 <= sum * 1.1f)
            //				t2 = sum;
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

    factory = new SceneFactory();
    transition = std::make_unique<CScreenTransition>();
    currentScene = factory->CreateFirst();
}

void SceneManager::Update()
{
    timeUpdate();

    // トランジション更新
    if (transition)
        transition->Update(deltaTime);

    // シーン切り替え後にFadeIn開始
    if (waitingForFadeIn && nextName == currentName)
    {
        waitingForFadeIn = false;
        transition->StartFadeIn();
    }

    if (nextName != currentName)
    {
        if (currentScene != nullptr)
        {
            delete currentScene;
            currentScene = nullptr;
        }
        currentName = nextName;
        currentScene = factory->Create(nextName);
    }
    if (currentScene != nullptr)
        currentScene->Update();
}

void SceneManager::Draw()
{
    if (currentScene != nullptr)
        currentScene->Draw();

    // トランジションは最前面に描画
    if (transition)
        transition->Draw();
}

void SceneManager::Release()
{
    if (currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
    }
    transition.reset();
    delete factory;
}

SceneBase* SceneManager::CurrentScene()
{
    return currentScene;
}

void SceneManager::SetCurrentScene(SceneBase* scene)
{
    currentScene = scene;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
    nextName = sceneName;
}

void SceneManager::ChangeSceneWithTransition(const std::string& sceneName)
{
    if (transition->IsTransitioning())
        return; // トランジション中は無視

    pendingSceneName = sceneName;
    transition->StartFadeOut([&]()
    {
        // FadeOut完了時にシーン切り替え
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
