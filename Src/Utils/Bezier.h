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
