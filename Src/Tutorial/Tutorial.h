#pragma once
#include <unordered_map>

#include "TutorialState.h"
#include "../Common/Object3D.h"

// 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｮ騾ｲ陦後ｒ邂｡逅・☆繧九け繝ｩ繧ｹ //
class CTutorial : public Object3D
{
public:
    CTutorial();
    // 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｮ繧ｹ繝・・繝医ｒ螟画峩縺吶ｋ
    // @param state 螟画峩蜈医・繧ｹ繝・・繝・//
    void SetState(CTutorialState::State state);

    // 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ蜍慕黄繧定､・焚菴鍋函謌舌☆繧・//
    void SpawnAnimal();

    // 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ莠ｺ髢薙→蜍慕黄繧堤函謌舌☆繧・//
    void SpawnHuman();

    // 螳溯ｷｵ蠖｢蠑上・繝ｬ繝・せ繝ｳ繧帝幕蟋九☆繧・//
    void Lesson();

private:

    void Update() override;

    CTutorialState* m_pCurrentState;
    std::unordered_map<CTutorialState::State, std::unique_ptr<CTutorialState>> m_states;
};
