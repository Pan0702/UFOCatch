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
// 繝ｪ繧ｶ繝ｫ繝医す繝ｼ繝ｳ縺ｸ驕ｷ遘ｻ縺吶ｋ //
////////////////////
void PlayScene::ChangeResultScene()
{
    SceneManager::ChangeSceneWithTransition("ResultScene");
    m_pBGM->Stop();
}
//4蛻・惠縺ｮ險育ｮ鈴㍼逶ｮ隕縫ebug
void PlayScene::QTreeDebug()
{
    
    // 蝗帛・譛ｨ縺ｮ蠖薙◆繧雁愛螳夂ｵｱ險医ｒ陦ｨ遉ｺ//
    CEnemyManager* pAnimalManager = ObjectManager::FindGameObject<CEnemyManager>();
    if (pAnimalManager != nullptr)
    {
        const CollisionStats& stats = pAnimalManager->GetCollisionStats();

        ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("_蝗帛・譛ｨ 蠖薙◆繧雁愛螳夂ｵｱ險・", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Text("=== 繝代ヵ繧ｩ繝ｼ繝槭Φ繧ｹ ===");
            ImGui::Text("Avg譎る俣: %.4f ms", stats.avgProcessTimeMs);
            ImGui::Text("譛螟ｧ譎る俣: %.4f ms", stats.maxProcessTimeMs);
            ImGui::Separator();

            ImGui::Text("=== 陦晉ｪ∝愛螳壼屓謨ｰ ===");
            ImGui::Text("謨ｵ縺ｮ謨ｰ: %d", stats.enemyCount);
            ImGui::Text("螳滄圀縺ｮ蛻､螳壼屓謨ｰ: %d", stats.totalChecks);
            ImGui::Text("_邱丞ｽ薙◆繧雁愛螳壼屓謨ｰ_: %d", stats.potentialChecks);
            ImGui::Text("_蜑頑ｸ帷紫_: %.1f%%", stats.reductionRate);
            ImGui::Separator();

            if (stats.potentialChecks > 0)
            {
                float efficiency = 100.0f * static_cast<float>(stats.totalChecks) / static_cast<float>(stats.potentialChecks);
                ImGui::Text("蜉ｹ邇・ %.1f%%", efficiency);
                ImGui::ProgressBar(efficiency / 100.0f, ImVec2(-1, 0), "");
            }

            if (ImGui::Button("_邨ｱ險医Μ繧ｻ繝・ヨ_"))
            {
                pAnimalManager->ResetCollisionStats();
            }
        }
        ImGui::End();
    }
}
