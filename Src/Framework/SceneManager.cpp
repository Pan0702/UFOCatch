#include <algorithm>
#include <cassert>
#include <Windows.h>
#include <unordered_map>
#include "sceneManager.h"
#include "sceneBase.h"
#include "SceneFactory.h"
#include "../Common/Constants.h"
#include "../MapEditor/ExportData.h"
#include "../MapEditor/Import.h"
#include "../Utils/ScreenTransition.h"

using namespace Constants;
namespace
{
    std::string currentName;
    std::string nextName;
    std::unique_ptr<SceneBase> currentScene;
    std::unique_ptr<SceneFactory> factory;
    std::unordered_map<BYTE,std::string> sceneTable; 
    
    // トランジション（画面遷移演出）
    std::unique_ptr<CScreenTransition> transition;
    std::string pendingSceneName; // トランジション中の次のシーン名
    bool waitingForFadeIn = false; // フェードイン待ち状態

    // DeltaTime（フレーム間経過時間）用
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
        
        // deltaTimeは、平均フレームレートの3倍を超えないように制限する（スパイク対策）
        if (recCount >= REC_SIZE)
        {
            float sum = 0;
            for (float i : record)
            {
                sum += i;
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
    DebugSceneInit();
    currentScene = factory->CreateFirst();
}

void SceneManager::Update()
{
    timeUpdate();
#ifdef _DEBUG 
    DebugInput();
#endif _DEBUG 

    // トランジション更新
    if (transition)
        transition->Update(deltaTime);

    // シーンの切り替え判定
    if (nextName != currentName)
    {
        if (currentScene != nullptr)
        {
            if (currentName == SceneName::EDITOR)                                
            {   
                ExportData::AllModelsPath(FileName::MODEL_LIST);
            }
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

void SceneManager::SetCurrentScene(std::unique_ptr<SceneBase> scene)
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
        return; // トランジション中は入力を無視

    pendingSceneName = sceneName;
    transition->StartFadeOut([&]()
    {
        // FadeOut完了時にシーン切り替えを実行
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

void SceneManager::DebugInput()
{
    auto input = GameDevice()->m_pDI;
    for (auto s : sceneTable)
    {
        if (input->CheckKey(KD_TRG, s.first))       
        {
            ChangeSceneWithTransition(s.second);    
            return ;
        }
    }
}

void SceneManager::DebugSceneInit()
{
  
    sceneTable.emplace(DIK_1, SceneName::TITLE);
    sceneTable.emplace(DIK_2, SceneName::SELECT);
    sceneTable.emplace(DIK_3, SceneName::EASY);
    sceneTable.emplace(DIK_4, SceneName::NORMAL);
    sceneTable.emplace(DIK_5, SceneName::RESULT);
    sceneTable.emplace(DIK_6, SceneName::TUTORIAL);
    sceneTable.emplace(DIK_7, SceneName::OI);
    sceneTable.emplace(DIK_8, SceneName::EDITOR);
    sceneTable.emplace(DIK_9, SceneName::DEBUG);
}