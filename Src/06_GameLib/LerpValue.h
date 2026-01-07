#pragma once
#include "Lerp.h"

/// 単一の値をLerpするための汎用構造体
struct LerpValue
{
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
    
    /// 値を強制的に設定し、Lerpを停止させる
    void ForceSetValue(float val)
    {
        start = val;
        target = val;
        timer = 0;
        duration = 0;
        isLerping = false;
    }
    
    bool IsLerping() const { return isLerping; }
private:
    float start;      // 開始値 //
    float target;     // 目標値 //
    float timer;      // 経過時間 //
    float duration;   // 補間にかける総時間 //
    bool isLerping;   // 補間中かどうか //
};


struct LerpValueVec3 {

    LerpValueVec3() : start(VECTOR3(0, 0, 0)), target(VECTOR3(0, 0, 0)){
    }

    /// Lerpを開始
    void Start(const VECTOR3 &from, const VECTOR3 &to, float dur) {
        x.Start(from.x, to.x, dur);
        y.Start(from.y, to.y, dur);
        z.Start(from.z, to.z, dur);
    }

    /// 毎フレーム更新して現在の値を返す
    VECTOR3 Update(float deltaTime) {
        return VECTOR3(x.Update(deltaTime),
                       y.Update(deltaTime),
                       z.Update(deltaTime));
    }

    bool IsLerping() const {
        return x.IsLerping() &&
               y.IsLerping() &&
               z.IsLerping();
    }
    
private:
    VECTOR3 start;
    VECTOR3 target;
    LerpValue x;
    LerpValue y;
    LerpValue z;
};