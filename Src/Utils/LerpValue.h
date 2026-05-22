#pragma once
#include "Lerp.h"
#include "Bezier.h"

/// 単一の値をLerpするための汎用構造体
struct LerpValue
{
    /// LerpValue を初期化する
    LerpValue() : start(0), target(0), timer(0), duration(0), isLerping(false)
    {
    }

    /// 開始する
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @param dur dur に渡す値
    void Start(float from, float to, float dur)
    {
        start = from;
        target = to;
        duration = dur;
        timer = 0.0f;
        isLerping = true;
    }

    /// 毎フレームの状態を更新する
    /// @param deltaTime 経過時間[秒]
    /// @return 計算結果の値
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

    /// Force Set Value の処理を行う
    /// @param val val に渡す値
    void ForceSetValue(float val)
    {
        start = val;
        target = val;
        timer = 0;
        duration = 0;
        isLerping = false;
    }

    /// Animating を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsAnimating() const { return isLerping; }

private:
    float start; // 開始値 //
    float target; // 目標値 //
    float timer; // 経過時間 //
    float duration; // 補間にかける総時間 //
    bool isLerping; // 補間中かどうか //
};


/// <summary>汎用ユーティリティで使う Lerp Value Vec3 の情報と処理をまとめる型</summary>
struct LerpValueVec3
{
    /// LerpValueVec3 を初期化する
    LerpValueVec3() : start(VECTOR3(0, 0, 0)), target(VECTOR3(0, 0, 0))
    {
    }

    /// 開始する
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @param dur dur に渡す値
    void Start(const VECTOR3& from, const VECTOR3& to, float dur)
    {
        x.Start(from.x, to.x, dur);
        y.Start(from.y, to.y, dur);
        z.Start(from.z, to.z, dur);
    }

    /// 毎フレームの状態を更新する
    /// @param deltaTime 経過時間[秒]
    /// @return 3次元ベクトル
    VECTOR3 Update(float deltaTime)
    {
        return VECTOR3(x.Update(deltaTime),
                       y.Update(deltaTime),
                       z.Update(deltaTime));
    }

    /// Animating を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsAnimating() const
    {
        return x.IsAnimating() &&
            y.IsAnimating() &&
            z.IsAnimating();
    }

private:
    VECTOR3 start;
    VECTOR3 target;
    LerpValue x;
    LerpValue y;
    LerpValue z;
};

/// 3次ベジエ曲線でVECTOR3を補間するための構造体
struct BezierValueVec3
{
    /// BezierValueVec3 を初期化する
    BezierValueVec3() : p0(VECTOR3(0, 0, 0)), p1(VECTOR3(0, 0, 0)),
                        p2(VECTOR3(0, 0, 0)), p3(VECTOR3(0, 0, 0)),
                        timer(0), duration(0), isAnimating(false)
    {
    }


    /// 開始する
    /// @param from from に渡す値
    /// @param controlPoint1 controlPoint1 に渡す値
    /// @param controlPoint2 controlPoint2 に渡す値
    /// @param to to に渡す値
    /// @param dur dur に渡す値
    void Start(const VECTOR3& from, const VECTOR3& controlPoint1,
               const VECTOR3& controlPoint2, const VECTOR3& to, float dur)
    {
        p0 = from;
        p1 = controlPoint1;
        p2 = controlPoint2;
        p3 = to;
        duration = dur;
        timer = 0.0f;
        isAnimating = true;
    }

    /// 毎フレームの状態を更新する
    /// @param deltaTime 経過時間[秒]
    /// @return 3次元ベクトル
    VECTOR3 Update(float deltaTime)
    {
        timer += deltaTime;
        float t = timer / duration;

        // tを0.0~1.0にクランプして、確実に終点に到達させる
        if (t >= 1.0f)
        {
            t = 1.0f;
            isAnimating = false;
            return p3; // 確実に終点を返す
        }

        return CubicBezier(p0, p1, p2, p3, t);
    }

    /// Animating を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsAnimating() const { return isAnimating; }

    /// Target を取得する
    /// @return 3次元ベクトル
    const VECTOR3& GetTarget() const { return p3; }

    /// Force Set Value の処理を行う
    /// @param val val に渡す値
    void ForceSetValue(const VECTOR3& val)
    {
        p0 = val;
        p3 = val;
        timer = 0;
        duration = 0;
        isAnimating = false;
    }

private:
    VECTOR3 p0; // 始点
    VECTOR3 p1; // 制御点1
    VECTOR3 p2; // 制御点2
    VECTOR3 p3; // 終点
    float timer; // 経過時間

    float duration; // アニメーション時間
    bool isAnimating; // アニメーション中かどうか
};
