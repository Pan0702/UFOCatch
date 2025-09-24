//-----------------------------------------------------------------------------
//
//  Direct3Dを利用するための汎用３Ｄライブラリ         ver 4.0        2024.12.3
// 
//	各種　数学系のライブラリ
//	(角度はラジアン角)
//
//																MyMath.h
//
//-----------------------------------------------------------------------------
#pragma once

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include <random>
#include <tchar.h>
#include <DirectXMath.h>
using namespace DirectX;	// DirectXMathの関数を使うためのネームスペース

//------------------------------------------------------------------------------
//
//  VECTOR2, VECTOR3, VECTOR4, MATRIX4X4 型の定義
//
//
//------------------------------------------------------------------------------

//
//  VECTOR4 型の定義
//
struct VECTOR4 : public XMFLOAT4
{
	//コンストラクタ
	VECTOR4() = default;
	VECTOR4(float x, float y, float z, float w)
	{
		this->x = x; this->y = y; this->z = z; this->w = w;
	}
	VECTOR4(const XMVECTOR& other) :XMFLOAT4()
	{
		XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
	}

	// 演算
	inline bool operator == (const VECTOR4& r) const { return x == r.x && y == r.y && z == r.z && w == r.w; }
	inline bool operator != (const VECTOR4& r) const { return x != r.x || y != r.y || z != r.z || w != r.w; }
	inline VECTOR4 operator +(const VECTOR4& r) const { return VECTOR4(x + r.x, y + r.y, z + r.z, w + r.w); }
	inline VECTOR4 operator -(const VECTOR4& r) const { return VECTOR4(x - r.x, y - r.y, z - r.z, w - r.w); }
	inline VECTOR4 operator +=(const VECTOR4& r) {
		x += r.x, y += r.y, z += r.z, w += r.w;
		return *this;
	}
	inline VECTOR4 operator -=(const VECTOR4& r) {
		x -= r.x, y -= r.y, z -= r.z, w -= r.w;
		return *this;
	}
	inline VECTOR4 operator *(const float& r) const { return VECTOR4(x * r, y * r, z * r, w * r); }
	inline VECTOR4 operator /(const float& r) const { return VECTOR4(x / r, y / r, z / r, w / r); }
	inline VECTOR4 operator *=(const float& r) {
		x *= r, y *= r, z *= r, w *= r;
		return *this;
	}
	inline VECTOR4 operator /=(const float& r) {
		x /= r, y /= r, z /= r, w /= r;
		return *this;
	}

	inline VECTOR4 operator + () const { return *this; }
	inline VECTOR4 operator - () const { return VECTOR4(-x, -y, -z, -w); }


	// 代入
	VECTOR4& operator=(const XMVECTOR& other)
	{
		XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
		return *this;
	}
	VECTOR4& operator=(const VECTOR4& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
		return *this;
	}

	// キャスト
	operator XMVECTOR() const {
		return XMLoadFloat4(this);
	}
	operator XMFLOAT4() const {
		return XMFLOAT4(this->x, this->y, this->z, this->w);
	}
};

//
//  VECTOR3 型の定義
//
struct VECTOR3 : public XMFLOAT3
{
	//コンストラクタ
	VECTOR3() = default;
	VECTOR3(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}
	VECTOR3(const XMVECTOR& other) :XMFLOAT3()
	{
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}

	// 演算
	inline bool operator == (const VECTOR3& r) const { return x == r.x && y == r.y && z == r.z; }
	inline bool operator != (const VECTOR3& r) const { return x != r.x || y != r.y || z != r.z; }
	inline VECTOR3 operator +(const VECTOR3& r) const { return VECTOR3(x + r.x, y + r.y, z + r.z); }
	inline VECTOR3 operator -(const VECTOR3& r) const { return VECTOR3(x - r.x, y - r.y, z - r.z); }
	inline VECTOR3 operator +=(const VECTOR3& r) {
		x += r.x, y += r.y, z += r.z;
		return *this;
	}
	inline VECTOR3 operator -=(const VECTOR3& r) {
		x -= r.x, y -= r.y, z -= r.z;
		return *this;
	}
	inline VECTOR3 operator *(const float& r) const { return VECTOR3(x * r, y * r, z * r); }
	inline VECTOR3 operator /(const float& r) const { return VECTOR3(x / r, y / r, z / r); }
	inline VECTOR3 operator *=(const float& r) {
		x *= r, y *= r, z *= r;
		return *this;
	}
	inline VECTOR3 operator /=(const float& r) {
		x /= r, y /= r, z /= r;
		return *this;
	}

	inline VECTOR3 operator + () const { return *this; }
	inline VECTOR3 operator - () const { return VECTOR3(-x, -y, -z); }


	// 代入
	VECTOR3& operator=(const XMVECTOR& other)
	{
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}
	VECTOR3& operator=(const VECTOR3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	// キャスト
	operator XMVECTOR() const {
		return XMLoadFloat3(this);
	}
	operator XMFLOAT3() const {
		return XMFLOAT3(this->x, this->y, this->z);
	}

	float LengthSquare() const { return this->x * this->x + this->y * this->y + this->z * this->z; }
	float Length() const { return sqrtf(LengthSquare()); }
};

//
//  VECTOR2 型の定義
//
struct VECTOR2 : public XMFLOAT2
{
	//コンストラクタ
	VECTOR2() = default;
	VECTOR2(float x, float y)
	{
		this->x = x; this->y = y;
	}
	VECTOR2(const XMVECTOR& other) :XMFLOAT2()
	{
		XMVECTOR temp = other;
		XMStoreFloat2(this, temp);
	}

	// 演算
	inline bool operator == (const VECTOR2& r) const { return x == r.x && y == r.y; }
	inline bool operator != (const VECTOR2& r) const { return x != r.x || y != r.y; }
	inline VECTOR2 operator +(const VECTOR2& r) const { return VECTOR2(x + r.x, y + r.y); }
	inline VECTOR2 operator -(const VECTOR2& r) const { return VECTOR2(x - r.x, y - r.y); }
	inline VECTOR2 operator +=(const VECTOR2& r) {
		x += r.x, y += r.y;
		return *this;
	}
	inline VECTOR2 operator -=(const VECTOR2& r) {
		x -= r.x, y -= r.y;
		return *this;
	}

	inline VECTOR2 operator *(const float& r) const { return VECTOR2(x * r, y * r); }
	inline VECTOR2 operator /(const float& r) const { return VECTOR2(x / r, y / r); }
	inline VECTOR2 operator *=(const float& r) {
		x *= r, y *= r;
		return *this;
	}
	inline VECTOR2 operator /=(const float& r) {
		x /= r, y /= r;
		return *this;
	}

	inline VECTOR2 operator + () const { return *this; }
	inline VECTOR2 operator - () const { return VECTOR2(-x, -y); }

	// 代入
	VECTOR2& operator=(const XMVECTOR& other)
	{
		XMVECTOR temp = other;
		XMStoreFloat2(this, temp);
		return *this;
	}
	VECTOR2& operator=(const VECTOR2& other)
	{
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	// キャスト
	operator XMVECTOR() const {
		return XMLoadFloat2(this);
	}
	operator XMFLOAT2() const {
		return XMFLOAT2(this->x, this->y);
	}
};

//
//  MATRIX4X4 型の定義
//
struct MATRIX4X4 : public XMFLOAT4X4
{
	//コンストラクタ
	MATRIX4X4() = default;
	MATRIX4X4(const XMMATRIX& other) :XMFLOAT4X4()
	{
		XMMATRIX temp = other;
		XMStoreFloat4x4(this, temp);
	}
	MATRIX4X4(float in_11,
		float in_12,
		float in_13,
		float in_14,
		float in_21,
		float in_22,
		float in_23,
		float in_24,
		float in_31,
		float in_32,
		float in_33,
		float in_34,
		float in_41,
		float in_42,
		float in_43,
		float in_44)
	{
		this->_11 = in_11;
		this->_12 = in_12;
		this->_13 = in_13;
		this->_14 = in_14;
		this->_21 = in_21;
		this->_22 = in_22;
		this->_23 = in_23;
		this->_24 = in_24;
		this->_31 = in_31;
		this->_32 = in_32;
		this->_33 = in_33;
		this->_34 = in_34;
		this->_41 = in_41;
		this->_42 = in_42;
		this->_43 = in_43;
		this->_44 = in_44;
	}

	// 代入
	inline MATRIX4X4& operator=(const XMMATRIX& other)
	{
		XMMATRIX temp = other;
		XMStoreFloat4x4(this, temp);
		return *this;
	}

	inline MATRIX4X4& operator=(const XMFLOAT4X4& other)
	{
		this->_11 = other._11;
		this->_12 = other._12;
		this->_13 = other._13;
		this->_14 = other._14;
		this->_21 = other._21;
		this->_22 = other._22;
		this->_23 = other._23;
		this->_24 = other._24;
		this->_31 = other._31;
		this->_32 = other._32;
		this->_33 = other._33;
		this->_34 = other._34;
		this->_41 = other._41;
		this->_42 = other._42;
		this->_43 = other._43;
		this->_44 = other._44;
		return *this;
	}

	inline MATRIX4X4& operator=(const MATRIX4X4& other)
	{
		this->_11 = other._11;
		this->_12 = other._12;
		this->_13 = other._13;
		this->_14 = other._14;
		this->_21 = other._21;
		this->_22 = other._22;
		this->_23 = other._23;
		this->_24 = other._24;
		this->_31 = other._31;
		this->_32 = other._32;
		this->_33 = other._33;
		this->_34 = other._34;
		this->_41 = other._41;
		this->_42 = other._42;
		this->_43 = other._43;
		this->_44 = other._44;
		return *this;
	}

	// 演算
	inline MATRIX4X4 operator *(const MATRIX4X4& r) const
	{
		XMMATRIX left = *this;
		XMMATRIX right = r;
		MATRIX4X4 ans;
		XMStoreFloat4x4(&ans, left * right);
		return ans;
	}

	// キャスト
	inline operator XMMATRIX() const {
		return XMLoadFloat4x4(this);
	}
	inline operator XMFLOAT4X4() const {
		XMFLOAT4X4 out;
		out._11 = this->_11;
		out._12 = this->_12;
		out._13 = this->_13;
		out._14 = this->_14;
		out._21 = this->_21;
		out._22 = this->_22;
		out._23 = this->_23;
		out._24 = this->_24;
		out._31 = this->_31;
		out._32 = this->_32;
		out._33 = this->_33;
		out._34 = this->_34;
		out._41 = this->_41;
		out._42 = this->_42;
		out._43 = this->_43;
		out._44 = this->_44;
		return out;
	}

};

inline const VECTOR3 operator *(const VECTOR3& vec, const MATRIX4X4& mat) {
	return XMVector3Transform(vec, mat);
}

inline const VECTOR3 operator *=(VECTOR3& vec, const MATRIX4X4& mat) {
	vec = XMVector3Transform(vec, mat);
	return vec;
}

inline float Dot(const VECTOR3& v1, const VECTOR3& v2) {
	VECTOR3 d = XMVector3Dot(v1, v2);
	return d.x;
}

// -----------------------------------------------------------------------------
// 汎用の３Ｄ算術ライブラリ
// -----------------------------------------------------------------------------

MATRIX4X4 GetLookatMatrix(const VECTOR3& vHear, const VECTOR3& vLookat);
VECTOR3   GetLookatRotateVector(const VECTOR3& startIn, const VECTOR3& targetIn);
VECTOR3   GetRotateVector(const MATRIX4X4& mat);
VECTOR3   GetScaleVector(const MATRIX4X4& mat);

MATRIX4X4 GetRotateMatrixX(const MATRIX4X4& mat);
MATRIX4X4 GetRotateMatrixY(const MATRIX4X4& mat);
MATRIX4X4 GetRotateMatrixZ(const MATRIX4X4& mat);
MATRIX4X4 GetRotateMatrix(const MATRIX4X4& mat);
VECTOR3   GetPositionVector(const MATRIX4X4& mat);
MATRIX4X4 GetPositionMatrix(const MATRIX4X4& mat);
VECTOR3 GetTargetRotateVector(const MATRIX4X4& mWorld, const VECTOR3& vTarget);

FLOAT avoidZero(FLOAT inp);
FLOAT dot(const VECTOR2& vLhs, const VECTOR2& vRhs);
FLOAT dot(const VECTOR3& vLhs, const VECTOR3& vRhs);
VECTOR3 cross(const VECTOR3& vLhs, const VECTOR3& vRhs);
FLOAT crossZ(const VECTOR2& vLhs, const VECTOR2& vRhs);
FLOAT magnitude(const VECTOR2& vLen);
FLOAT magnitude(const VECTOR3& vLen);
FLOAT magnitudeSQ(const VECTOR3& vLen);
VECTOR2 scaling(const VECTOR2& vVec, const FLOAT& Scale);
VECTOR3 scaling(const VECTOR3& vVec, const FLOAT& Scale);
VECTOR2 normalize(const VECTOR2& vVec);
VECTOR3 normalize(const VECTOR3& vVec);
FLOAT Angle(const VECTOR3& v1, const VECTOR3& v2);
FLOAT AddAngle(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3);
FLOAT AddAngle(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3, const VECTOR3& p4);

int Random(int min1, int max1);
float Randomf(float min1, float max1);

static const float DegToRad = XM_PI /180.0f;
static const float RadToDeg = 180.0f / XM_PI;
