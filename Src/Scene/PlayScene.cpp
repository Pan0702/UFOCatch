#include "PlayScene.h"
#include <cassert>

#include "../System/GameInstance.h"
#include "../Enemies/System/EnemyManager.h"
#include "../Player/PCamera.h"
#include "../Player/Player.h"
#include "../Enemies/AnimalDog/Dog.h"
#include "../Enemies/System/AnimalFactor.h"
#include "../Enemies/System/Flog.h"
#include "../System/DisplayInfo.h"
#include "../System/VisionSystem.h"
#include "../Stage/StageFactor.h"
#include "../System/Timer.h"

PlayScene::PlayScene()
{
    Instantiate<CStageFactor>();
    ObjectManager::FindGameObject<CStageFactor>()->SpawnObjects(30, 30,90);
    Instantiate<CPlayerCamera>();
    SingleInstantiate<CEnemyManager>();
    ObjectManager::FindGameObject<CEnemyManager>()->BuildStaticTree();
    Instantiate<CTimer>(30);
    Instantiate<CPlayer>(25);             
    Instantiate<CAnimalFactor>(20,20,0);
   // Instantiate<CFlog>();           
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    CGameInstance::Get()->Init(3000 );
    AudioManager::Load(_T("Play"), _T("data/Sound/yukai.wav"));
    AudioManager::Play(_T("Play"));
}

PlayScene::~PlayScene()
{
    AudioManager::Stop(_T("Play"));
}

void PlayScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T))
    {
        SceneManager::ChangeScene("TitleScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
    {
        SceneManager::ChangeScene("ResultScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
    {
        SceneManager::ChangeScene("SelectScene");
    }
}

void PlayScene::Draw()
{
    GameDevice()->m_pFont->Draw(
        20, 20, "PlayScene", 16, RGB(255, 255, 0));
    //Debug
    QTreeDebug();
}

////////////////////
// リザルトシーンへ遷移する //
////////////////////
void PlayScene::ChangeResultScene()
{
    SceneManager::ChangeSceneWithTransition("ResultScene");
    m_pBGM->Stop();
}
//4分木の計算量目視Debug
void PlayScene::QTreeDebug()
{
    
    // 四分木の当たり判定統計を表示//
    CEnemyManager* pAnimalManager = ObjectManager::FindGameObject<CEnemyManager>();
    if (pAnimalManager != nullptr)
    {
        const CollisionStats& stats = pAnimalManager->GetCollisionStats();

        ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("_四分木 当たり判定統計_", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Text("=== パフォーマンス ===");
            ImGui::Text("Avg時間: %.4f ms", stats.avgProcessTimeMs);
            ImGui::Text("最大時間: %.4f ms", stats.maxProcessTimeMs);
            ImGui::Separator();

            ImGui::Text("=== 衝突判定回数 ===");
            ImGui::Text("敵の数: %d", stats.enemyCount);
            ImGui::Text("実際の判定回数: %d", stats.totalChecks);
            ImGui::Text("_総当たり判定回数_: %d", stats.potentialChecks);
            ImGui::Text("_削減率_: %.1f%%", stats.reductionRate);
            ImGui::Separator();

            if (stats.potentialChecks > 0)
            {
                float efficiency = 100.0f * static_cast<float>(stats.totalChecks) / static_cast<float>(stats.potentialChecks);
                ImGui::Text("効率: %.1f%%", efficiency);
                ImGui::ProgressBar(efficiency / 100.0f, ImVec2(-1, 0), "");
            }

            if (ImGui::Button("_統計リセット_"))
            {
                pAnimalManager->ResetCollisionStats();
            }
        }
        ImGui::End();
    }
}
