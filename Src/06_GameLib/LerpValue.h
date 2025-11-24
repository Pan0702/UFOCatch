#pragma once
#include "Lerp.h"
#include "../04_FrameWork//SceneManager.h"

/// 単一の値をLerpするための汎用構造体
struct LerpValue
{
    float start;
    float target;
    float timer;
    float duration;
    bool isLerping;

    LerpValue() : start(0), target(0), timer(0), duration(0), isLerping(false) {}

    /// Lerpを開始
    void Start(float from, float to, float dur)
    {
        start = from;
        target = to;
        duration = dur;
        timer = 0.0f;
        isLerping = true;
    }

    /// 毎フレーム更新して現在の値を返す
    float Update(float deltaTime)
    {
        if (!isLerping) return target;

        timer += deltaTime;
        float t = timer / duration;

        if (t >= 1.0f)
        {
            isLerping = false;
            return target;
        }

        return Lerp(start, target, t);
    }
    
    bool IsLerping() const { return isLerping; }
};
