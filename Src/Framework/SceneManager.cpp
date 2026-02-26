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

    // トランジション
    std::unique_ptr<CScreenTransition> transition;
    std::string pendingSceneName; // トランジション中の次のシーン名
    bool waitingForFadeIn = false; // FadeIn待ち状態

    // DeltaTime用
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
        // deltaTimeは、平均フレームレートの2倍を超えないようにする
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

    // トランジション更新
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
        // シーン生成はロード処理で時間がかかる場合がある。
        // ロード時間を次フレームの deltaTime に含めないようタイムスタンプをリセットする。
        QueryPerformanceCounter(&current);
    }

    // シーン切り替え後にFadeIn開始
    if (waitingForFadeIn && nextName == currentName)
    {
        waitingForFadeIn = false;
        // シーンのロード（currentSceneの生成）が完了した直後にFadeInを開始する
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
    // トランジションは最前面に描画（ObjectManager::Draw()より後に呼ぶこと）
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
