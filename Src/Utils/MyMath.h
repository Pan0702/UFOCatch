//-----------------------------------------------------------------------------
//
//  Direct3Dを利用するための汎用3Dライブラリ         ver 4.0        2024.12.3
// 
//  (角度の単位はラジアン)
//
//                                               MyMath.h
//
//-----------------------------------------------------------------------------
#pragma once

// ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include <random>
#include <tchar.h>
#include <DirectXMath.h>
using namespace DirectX; // DirectXMathの関数を使いやすくするための名前空間

//------------------------------------------------------------------------------
//
//  VECTOR2, VECTOR3, VECTOR4, MATRIX4X4 型の定義
//
//------------------------------------------------------------------------------

//
//  VECTOR4 型の定義
//
struct VECTOR4 : public XMFLOAT4
{
    // コンストラクタ
    VECTOR4() = default;

    VECTOR4(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    VECTOR4(const XMVECTOR& other) : XMFLOAT4()
    {
        XMVECTOR temp = other;
        XMStoreFloat4(this, temp);
    }

    // 演算子
    /// == を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline bool operator ==(const VECTOR4& r) const { return x == r.x && y == r.y && z == r.z && w == r.w; }
    /// != を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline bool operator !=(const VECTOR4& r) const { return x != r.x || y != r.y || z != r.z || w != r.w; }
    /// + を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator +(const VECTOR4& r) const { return VECTOR4(x + r.x, y + r.y, z + r.z, w + r.w); }
    /// - を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator -(const VECTOR4& r) const { return VECTOR4(x - r.x, y - r.y, z - r.z, w - r.w); }
    /// += を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator +=(const VECTOR4& r)
    {
        x += r.x, y += r.y, z += r.z, w += r.w;
        return *this;
    }

    /// -= を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator -=(const VECTOR4& r)
    {
        x -= r.x, y -= r.y, z -= r.z, w -= r.w;
        return *this;
    }

    ///  を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator *(const float& r) const { return VECTOR4(x * r, y * r, z * r, w * r); }
    /// / を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator /(const float& r) const { return VECTOR4(x / r, y / r, z / r, w / r); }
    /// = を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator *=(const float& r)
    {
        x *= r, y *= r, z *= r, w *= r;
        return *this;
    }

    /// /= を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR4 operator /=(const float& r)
    {
        x /= r, y /= r, z /= r, w /= r;
        return *this;
    }

    /// + を返す
    /// @return 処理結果
    inline VECTOR4 operator +() const { return *this; }
    /// - を返す
    /// @return 処理結果
    inline VECTOR4 operator -() const { return VECTOR4(-x, -y, -z, -w); }


    // 代入
    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 4次元ベクトル
    VECTOR4& operator=(const XMVECTOR& other)
    {
        XMVECTOR temp = other;
        XMStoreFloat4(this, temp);
        return *this;
    }

    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 4次元ベクトル
    VECTOR4& operator=(const VECTOR4& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
        return *this;
    }

    // キャスト
    operator XMVECTOR() const
    {
        return XMLoadFloat4(this);
    }

    operator XMFLOAT4() const
    {
        return XMFLOAT4(this->x, this->y, this->z, this->w);
    }
};

//
//  VECTOR3 型の定義
//
struct VECTOR3 : public XMFLOAT3
{
    // コンストラクタ
    VECTOR3() = default;

    VECTOR3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    VECTOR3(const XMVECTOR& other) : XMFLOAT3()
    {
        XMVECTOR temp = other;
        XMStoreFloat3(this, temp);
    }

    // 演算子
    // 演算
    /// == を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline bool operator ==(const VECTOR3& r) const { return x == r.x && y == r.y && z == r.z; }
    /// != を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline bool operator !=(const VECTOR3& r) const { return x != r.x || y != r.y || z != r.z; }
    /// + を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator +(const VECTOR3& r) const { return VECTOR3(x + r.x, y + r.y, z + r.z); }
    /// - を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator -(const VECTOR3& r) const { return VECTOR3(x - r.x, y - r.y, z - r.z); }
    /// += を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator +=(const VECTOR3& r)
    {
        x += r.x, y += r.y, z += r.z;
        return *this;
    }

    /// -= を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator -=(const VECTOR3& r)
    {
        x -= r.x, y -= r.y, z -= r.z;
        return *this;
    }

    ///  を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator *(const float& r) const { return VECTOR3(x * r, y * r, z * r); }
    /// / を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator /(const float& r) const { return VECTOR3(x / r, y / r, z / r); }
    /// = を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator *=(const float& r)
    {
        x *= r, y *= r, z *= r;
        return *this;
    }

    /// /= を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR3 operator /=(const float& r)
    {
        x /= r, y /= r, z /= r;
        return *this;
    }

    /// + を返す
    /// @return 処理結果
    inline VECTOR3 operator +() const { return *this; }
    /// - を返す
    /// @return 処理結果
    inline VECTOR3 operator -() const { return VECTOR3(-x, -y, -z); }


    // 代入

    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 3次元ベクトル
    VECTOR3& operator=(const XMVECTOR& other)
    {
        XMVECTOR temp = other;
        XMStoreFloat3(this, temp);
        return *this;
    }

    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 3次元ベクトル
    VECTOR3& operator=(const VECTOR3& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    // キャスト
    operator XMVECTOR() const
    {
        return XMLoadFloat3(this);
    }

    operator XMFLOAT3() const
    {
        return {this->x, this->y, this->z};
    }

    /// Length Square を返す
    /// @return 計算結果の値
    float LengthSquare() const { return this->x * this->x + this->y * this->y + this->z * this->z; }
    /// Length を返す
    /// @return 計算結果の値
    float Length() const { return sqrtf(LengthSquare()); }
    /// Abs を返す
    /// @return 3次元ベクトル
    VECTOR3 Abs() const { return VECTOR3(std::fabs(this->x), std::fabs(this->y), std::fabs(this->z)); }
};

//
//  VECTOR2 型の定義
//
struct VECTOR2 : public XMFLOAT2
{
    // コンストラクタ
    VECTOR2() = default;

    VECTOR2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    VECTOR2(const XMVECTOR& other) : XMFLOAT2()
    {
        XMVECTOR temp = other;
        XMStoreFloat2(this, temp);
    }

    // 演算子
    /// == を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline bool operator ==(const VECTOR2& r) const { return x == r.x && y == r.y; }
    /// != を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline bool operator !=(const VECTOR2& r) const { return x != r.x || y != r.y; }
    /// + を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator +(const VECTOR2& r) const { return VECTOR2(x + r.x, y + r.y); }
    /// - を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator -(const VECTOR2& r) const { return VECTOR2(x - r.x, y - r.y); }
    /// - を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator -(const float& r) const { return VECTOR2(x - r, y - r); }
    /// += を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator +=(const VECTOR2& r)
    {
        x += r.x, y += r.y;
        return *this;
    }

    /// -= を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator -=(const VECTOR2& r)
    {
        x -= r.x, y -= r.y;
        return *this;
    }

    ///  を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator *(const float& r) const { return VECTOR2(x * r, y * r); }
    /// / を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator /(const float& r) const { return VECTOR2(x / r, y / r); }
    /// = を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator *=(const float& r)
    {
        x *= r, y *= r;
        return *this;
    }

    /// /= を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline VECTOR2 operator /=(const float& r)
    {
        x /= r, y /= r;
        return *this;
    }

    /// + を返す
    /// @return 処理結果
    inline VECTOR2 operator +() const { return *this; }
    /// - を返す
    /// @return 処理結果
    inline VECTOR2 operator -() const { return VECTOR2(-x, -y); }

    // 代入
    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 2次元ベクトル
    VECTOR2& operator=(const XMVECTOR& other)
    {
        XMVECTOR temp = other;
        XMStoreFloat2(this, temp);
        return *this;
    }

    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 2次元ベクトル
    VECTOR2& operator=(const VECTOR2& other)
    {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }

    // キャスト
    operator XMVECTOR() const
    {
        return XMLoadFloat2(this);
    }

    operator XMFLOAT2() const
    {
        return XMFLOAT2(this->x, this->y);
    }
};

//
//  MATRIX4X4 型の定義
//
struct MATRIX4X4 : public XMFLOAT4X4
{
    // コンストラクタ
    MATRIX4X4() = default;

    MATRIX4X4(const XMMATRIX& other) : XMFLOAT4X4()
    {
        XMMATRIX temp = other;
        XMStoreFloat4x4(this, temp);
    }

    MATRIX4X4(float in_11, float in_12, float in_13, float in_14,
              float in_21, float in_22, float in_23, float in_24,
              float in_31, float in_32, float in_33, float in_34,
              float in_41, float in_42, float in_43, float in_44)
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
    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 行列
    inline MATRIX4X4& operator=(const XMMATRIX& other)
    {
        XMMATRIX temp = other;
        XMStoreFloat4x4(this, temp);
        return *this;
    }

    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 行列
    inline MATRIX4X4& operator=(const XMFLOAT4X4& other)
    {
        memcpy(this, &other, sizeof(XMFLOAT4X4));
        return *this;
    }

    /// operator= の処理を行う
    /// @param other other に渡す値
    /// @return 行列
    inline MATRIX4X4& operator=(const MATRIX4X4& other)
    {
        memcpy(this, &other, sizeof(MATRIX4X4));
        return *this;
    }

    // 演算子
    ///  を返す
    /// @param r r に渡す値
    /// @return 処理結果
    inline MATRIX4X4 operator *(const MATRIX4X4& r) const
    {
        XMMATRIX left = *this;
        XMMATRIX right = r;
        MATRIX4X4 ans;
        XMStoreFloat4x4(&ans, left * right);
        return ans;
    }

    // キャスト
    inline operator XMMATRIX() const
    {
        return XMLoadFloat4x4(this);
    }

    inline operator XMFLOAT4X4() const
    {
        XMFLOAT4X4 out;
        memcpy(&out, this, sizeof(XMFLOAT4X4));
        return out;
    }
};

///  を返す
/// @param vec vec に渡す値
/// @param mat mat に渡す値
/// @return 処理結果
inline const VECTOR3 operator *(const VECTOR3& vec, const MATRIX4X4& mat)
{
    return XMVector3Transform(vec, mat);
}

/// = を返す
/// @param vec vec に渡す値
/// @param mat mat に渡す値
/// @return 処理結果
inline const VECTOR3 operator *=(VECTOR3& vec, const MATRIX4X4& mat)
{
    vec = XMVector3Transform(vec, mat);
    return vec;
}

/// Dot を返す
/// @param v1 v1 に渡す値
/// @param v2 v2 に渡す値
/// @return 計算結果の値
inline float Dot(const VECTOR3& v1, const VECTOR3& v2)
{
    VECTOR3 d = XMVector3Dot(v1, v2);
    return d.x;
}

// -----------------------------------------------------------------------------
// 汎用の3D計算ライブラリ
// -----------------------------------------------------------------------------

/// Lookat Matrix を取得する
/// @param vHear vHear に渡す値
/// @param vLookat vLookat に渡す値
/// @return 行列
MATRIX4X4 GetLookatMatrix(const VECTOR3& vHear, const VECTOR3& vLookat);
/// Lookat Rotate Vector を取得する
/// @param startIn startIn に渡す値
/// @param targetIn 対象
/// @return 3次元ベクトル
VECTOR3 GetLookatRotateVector(const VECTOR3& startIn, const VECTOR3& targetIn);
/// Rotate Vector を取得する
/// @param mat mat に渡す値
/// @return 3次元ベクトル
VECTOR3 GetRotateVector(const MATRIX4X4& mat);
/// Scale Vector を取得する
/// @param mat mat に渡す値
/// @return 3次元ベクトル
VECTOR3 GetScaleVector(const MATRIX4X4& mat);

/// Rotate Matrix X を取得する
/// @param mat mat に渡す値
/// @return 行列
MATRIX4X4 GetRotateMatrixX(const MATRIX4X4& mat);
/// Rotate Matrix Y を取得する
/// @param mat mat に渡す値
/// @return 行列
MATRIX4X4 GetRotateMatrixY(const MATRIX4X4& mat);
/// Rotate Matrix Z を取得する
/// @param mat mat に渡す値
/// @return 行列
MATRIX4X4 GetRotateMatrixZ(const MATRIX4X4& mat);
/// Rotate Matrix を取得する
/// @param mat mat に渡す値
/// @return 行列
MATRIX4X4 GetRotateMatrix(const MATRIX4X4& mat);
/// Position Vector を取得する
/// @param mat mat に渡す値
/// @return 3次元ベクトル
VECTOR3 GetPositionVector(const MATRIX4X4& mat);
/// Position Matrix を取得する
/// @param mat mat に渡す値
/// @return 行列
MATRIX4X4 GetPositionMatrix(const MATRIX4X4& mat);
/// Target Rotate Vector を取得する
/// @param mWorld mWorld に渡す値
/// @param vTarget 対象
/// @return 3次元ベクトル
VECTOR3 GetTargetRotateVector(const MATRIX4X4& mWorld, const VECTOR3& vTarget);

/// avoid Zero を返す
/// @param inp inp に渡す値
/// @return 処理結果
FLOAT avoidZero(FLOAT inp);
/// dot を返す
/// @param vLhs vLhs に渡す値
/// @param vRhs vRhs に渡す値
/// @return 処理結果
FLOAT dot(const VECTOR2& vLhs, const VECTOR2& vRhs);
/// dot を返す
/// @param vLhs vLhs に渡す値
/// @param vRhs vRhs に渡す値
/// @return 処理結果
FLOAT dot(const VECTOR3& vLhs, const VECTOR3& vRhs);
/// cross を返す
/// @param vLhs vLhs に渡す値
/// @param vRhs vRhs に渡す値
/// @return 3次元ベクトル
VECTOR3 cross(const VECTOR3& vLhs, const VECTOR3& vRhs);
/// cross Z を返す
/// @param vLhs vLhs に渡す値
/// @param vRhs vRhs に渡す値
/// @return 処理結果
FLOAT crossZ(const VECTOR2& vLhs, const VECTOR2& vRhs);
/// magnitude を返す
/// @param vLen vLen に渡す値
/// @return 処理結果
FLOAT magnitude(const VECTOR2& vLen);
/// magnitude を返す
/// @param vLen vLen に渡す値
/// @return 処理結果
FLOAT magnitude(const VECTOR3& vLen);
/// magnitude SQ を返す
/// @param vLen vLen に渡す値
/// @return 処理結果
FLOAT magnitudeSQ(const VECTOR3& vLen);
/// scaling を返す
/// @param vVec vVec に渡す値
/// @param Scale スケール
/// @return 2次元ベクトル
VECTOR2 scaling(const VECTOR2& vVec, const FLOAT& Scale);
/// scaling を返す
/// @param vVec vVec に渡す値
/// @param Scale スケール
/// @return 3次元ベクトル
VECTOR3 scaling(const VECTOR3& vVec, const FLOAT& Scale);
/// normalize を返す
/// @param vVec vVec に渡す値
/// @return 2次元ベクトル
VECTOR2 normalize(const VECTOR2& vVec);
/// normalize を返す
/// @param vVec vVec に渡す値
/// @return 3次元ベクトル
VECTOR3 normalize(const VECTOR3& vVec);
/// Angle を返す
/// @param v1 v1 に渡す値
/// @param v2 v2 に渡す値
/// @return 処理結果
FLOAT Angle(const VECTOR3& v1, const VECTOR3& v2);
/// Angle を追加する
/// @param p1 p1 に渡す値
/// @param p2 p2 に渡す値
/// @param p3 p3 に渡す値
/// @return 処理結果
FLOAT AddAngle(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3);
/// Angle を追加する
/// @param p1 p1 に渡す値
/// @param p2 p2 に渡す値
/// @param p3 p3 に渡す値
/// @param p4 p4 に渡す値
/// @return 処理結果
FLOAT AddAngle(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3, const VECTOR3& p4);

/// Random を返す
/// @param min1 min1 に渡す値
/// @param max1 max1 に渡す値
/// @return 処理結果の数値
int Random(int min1, int max1);
/// Randomf を返す
/// @param min1 min1 に渡す値
/// @param max1 max1 に渡す値
/// @return 計算結果の値
float Randomf(float min1, float max1);

static const float DegToRad = XM_PI / 180.0f;
static const float RadToDeg = 180.0f / XM_PI;

// -----------------------------------------------------------------------------
// 補助関数
// -----------------------------------------------------------------------------
/// Pow2 を返す
/// @param n n に渡す値
/// @return 計算結果の値
inline float Pow2(const float& n)
{
    return n * n;
}

/// Pow を返す
/// @param base base に渡す値
/// @param exp exp に渡す値
/// @return 計算結果の値
inline float Pow(float base, int exp)
{
    float result = 1.0f;
    for (int i = 0; i < exp; i++)
    {
        result *= base;
    }
    return result;
}

/**
 * 与えられた2つの3D点のx座標とz座標によって形成される直角三角形の斜辺を計算します。
 *
 * @param v1 最初の3Dベクトル
 * @param v2 2番目の3Dベクトル
 * @return 2つの点のx座標とz座標の差によって形成される直角三角形の斜辺の長さ
 */
/// Distance XZ を計算する
/// @param v1 v1 に渡す値
/// @param v2 v2 に渡す値
/// @return 計算結果の値
inline float CalcDistanceXZ(const VECTOR3& v1, const VECTOR3& v2)
{
    VECTOR2 distance = VECTOR2(v1.x - v2.x, v1.z - v2.z);
    return sqrtf(Pow2(distance.x) + Pow2(distance.y));
}

/// V3abs を返す
/// @param n n に渡す値
/// @return 3次元ベクトル
inline VECTOR3 V3abs(const VECTOR3& n)
{
    VECTOR3 tmp;
    tmp.x = std::fabs(n.x);
    tmp.y = std::fabs(n.y);
    tmp.z = std::fabs(n.z);
    return tmp;
}

/// Cosine Formula を計算する
/// @param A A に渡す値
/// @param B B に渡す値
/// @param C C に渡す値
/// @return 計算結果の値
inline float CalcCosineFormula(const float& A, const float& B, const float& C)
{
    if (A == 0 || B == 0)
    {
        return 0;
    }
    float formula = (Pow2(A) + Pow2(B) - Pow2(C)) / (2 * A * B);
    return acosf(formula) * RadToDeg;
}

/// Vector2 Angle を計算する
/// @param vNorm1 vNorm1 に渡す値
/// @param vNorm2 vNorm2 に渡す値
/// @return 計算結果の値
inline float CalcVector2Angle(const VECTOR2& vNorm1, const VECTOR2& vNorm2)
{
    float angle = acosf(dot(vNorm1, vNorm2));
    return angle * RadToDeg;
}

/// My Ramdom を返す
/// @param min min に渡す値
/// @param max max に渡す値
/// @param gen gen に渡す値
/// @return 計算結果の値
inline float MyRamdom(const float& min, const float& max, std::mt19937& gen)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

/// Inside Area XZ を判定する
/// @param pos 座標
/// @param size サイズ
/// @return 成功または条件を満たす場合 true
inline bool IsInsideAreaXZ(const VECTOR3& pos, const VECTOR2& size)
{
    if ((pos.x >= -size.x && pos.x <= size.x)
        && (pos.z >= -size.y && pos.z <= size.y))
    {
        return true;
    }
    return false;
}

/// To Vec2 XZ を返す
/// @param pos 座標
/// @return 2次元ベクトル
inline VECTOR2 ToVec2XZ(const VECTOR3& pos)
{
    return VECTOR2(pos.x, pos.z);
}

/// Clamp を返す
/// @param num num に渡す値
/// @param max max に渡す値
/// @return 処理結果の数値
inline int Clamp(int num, int max)
{
    if (num > max)
    {
        return max;
    }
    if (num < 0)
    {
        return 0;
    }
    return num;
}
