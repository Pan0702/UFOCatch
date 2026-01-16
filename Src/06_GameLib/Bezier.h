#pragma once
#include "MyMath.h"

// -----------------------------------------------------------------------------
// Bezier Curve Functions
// -----------------------------------------------------------------------------

/**
 * 3次ベジエ曲線上の点を計算
 * @param p0 始点
 * @param p1 制御点1
 * @param p2 制御点2
 * @param p3 終点
 * @param t 補間係数 (0.0 ~ 1.0)
 * @return 曲線上の点
 */
inline VECTOR3 CubicBezier(const VECTOR3& p0, const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3, float t)
{
	float oneMinusT = 1.0f - t;
	float oneMinusT2 = oneMinusT * oneMinusT;
	float oneMinusT3 = oneMinusT2 * oneMinusT;
	float t2 = t * t;
	float t3 = t2 * t;

	return p0 * oneMinusT3 +
	       p1 * (3.0f * oneMinusT2 * t) +
	       p2 * (3.0f * oneMinusT * t2) +
	       p3 * t3;
}

/**
 * 弧を描くベジエ曲線の制御点を自動生成
 * 始点と終点の中間を上方向にオフセットして弧を作る
 * @param start 始点
 * @param end 終点
 * @param heightOffset 弧の高さオフセット（中間点をどれだけ上に持ち上げるか）
 * @param outP1 制御点1の出力
 * @param outP2 制御点2の出力
 */
inline void GenerateArcBezierControlPoints(const VECTOR3& start, const VECTOR3& end, float heightOffset,
                                           VECTOR3& outP1, VECTOR3& outP2)
{
	// 始点と終点の中間点を計算
	VECTOR3 mid = (start + end) * 0.5f;

	// 上方向に持ち上げた中間点を作る
	VECTOR3 arcTop = mid;
	arcTop.y += heightOffset;

	// 制御点を始点と終点から弧の頂点に向けて配置
	// 係数を0.5に近づけると弧が急になり、1.0に近づけると緩やかになる
	outP1 = start + (arcTop - start) * 0.5f;
	outP2 = end + (arcTop - end) * 0.5f;
}
