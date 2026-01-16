#pragma once
#include "Lerp.h"
#include "Bezier.h"

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

/// 3次ベジエ曲線でVECTOR3を補間するための構造体
struct BezierValueVec3 {
    BezierValueVec3() : p0(VECTOR3(0, 0, 0)), p1(VECTOR3(0, 0, 0)),
                        p2(VECTOR3(0, 0, 0)), p3(VECTOR3(0, 0, 0)),
                        timer(0), duration(0), isAnimating(false) {}

    /// ベジエ曲線アニメーションを開始（自動生成版）
    /// @param from 始点
    /// @param to 終点
    /// @param dur アニメーション時間
    /// @param heightOffset 弧の高さオフセット（中間点を上に持ち上げる量）
    void Start(const VECTOR3& from, const VECTOR3& to, float dur, float heightOffset) {
        p0 = from;
        p3 = to;
        duration = dur;
        timer = 0.0f;
        isAnimating = true;

        // 制御点を自動生成して弧を作る
        GenerateArcBezierControlPoints(from, to, heightOffset, p1, p2);
    }

    /// ベジエ曲線アニメーションを開始（制御点指定版）
    /// @param from 始点
    /// @param controlPoint1 制御点1
    /// @param controlPoint2 制御点2
    /// @param to 終点
    /// @param dur アニメーション時間
    void StartWithControlPoints(const VECTOR3& from, const VECTOR3& controlPoint1,
                                const VECTOR3& controlPoint2, const VECTOR3& to, float dur) {
        p0 = from;
        p1 = controlPoint1;
        p2 = controlPoint2;
        p3 = to;
        duration = dur;
        timer = 0.0f;
        isAnimating = true;
    }

    /// 毎フレーム更新して現在の位置を返す
    VECTOR3 Update(float deltaTime) {
        if (!isAnimating) return p3;

        timer += deltaTime;
        float t = timer / duration;

        // tを0.0~1.0にクランプして、確実に終点に到達させる
        if (t >= 1.0f) {
            t = 1.0f;
            isAnimating = false;
            return p3;  // 確実に終点を返す
        }

        // イージングを適用: 前半減速、後半加速
        float easedT = EaseSlowToFast(t);

        return CubicBezier(p0, p1, p2, p3, easedT);
    }

    /// アニメーション中かどうか
    bool IsAnimating() const { return isAnimating; }

    /// 値を強制的に設定し、アニメーションを停止
    void ForceSetValue(const VECTOR3& val) {
        p0 = val;
        p3 = val;
        timer = 0;
        duration = 0;
        isAnimating = false;
    }

private:
    VECTOR3 p0;         // 始点
    VECTOR3 p1;         // 制御点1
    VECTOR3 p2;         // 制御点2
    VECTOR3 p3;         // 終点
    float timer;        // 経過時間
    float duration;     // アニメーション時間
    bool isAnimating;   // アニメーション中かどうか
};