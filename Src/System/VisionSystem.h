#pragma once
#include "../Common/Object3D.h"

// 視界判定システムクラス（扇形と円の衝突判定を管理） //
class CVisionSystem : public Object3D
{
public:
    // コンストラクタ //
    CVisionSystem();

    // デストラクタ //
    ~CVisionSystem();

    // 指定された角度が扇形の視野角内にあるか判定
    // @param angle 判定する角度（ラジアン）
    // @return 角度が視野角内ならtrue、範囲外ならfalse //
    bool IsAngleInSector(const float& angle) const;

    // 扇形と円の衝突判定（人間の視界範囲内にプレイヤーがいるかチェック）
    // @param humanPos 人間の位置（2Dベクトル）
    // @param humanAngle 人間の向き（ラジアン）
    // @return 円が視界扇形内、または扇形の境界と交差している場合true、それ以外はfalse //
    bool SectorCircleCollision(const VECTOR2& humanPos, float humanAngle);
    
    // 円の中心座標を設定
    // @param pos 設定する3D座標 //
    void SetCircleCenter(const VECTOR3& pos);

    // 円の半径を設定
    // @param radius 設定する半径 //
    void SetCircleRadius(const float& radius);

private:
    // 線分と円の交差判定（扇形の辺と円が交わっているかチェック）
    // @param lineStart 線分の開始点
    // @param lineEnd 線分の終了点
    // @param circleCenter 円の中心座標
    // @param circleRadius 円の半径
    // @return 線分と円が交差している場合true、それ以外はfalse //
    bool LineSegmentCircleIntersection(const VECTOR2& lineStart, const VECTOR2& lineEnd, const VECTOR2& circleCenter, float circleRadius) const;
    
    // 円の情報を保持する構造体 //
    struct CircleInfo
    {
        VECTOR2 center;  // 円の中心座標 //
        float radius;    // 円の半径 //

        CircleInfo(const VECTOR2& pos_ = VECTOR2(0,0),const float& radius_ = 0.0f)
            : center(pos_),radius(radius_)
        {}

        // 3D座標からXZ平面の2D座標に変換して中心座標を設定
        // @param pos_ 3D座標 //
        void SetCenter(const VECTOR3& pos_) { center = ToVec2XZ(pos_); }

        // 円の半径を設定
        // @param radius_ 設定する半径 //
        void SetRadius(const float& radius_) { radius = radius_; }
    };

    CircleInfo m_circleInfo;  // プレイヤーの円情報 //

    // 扇形の情報を保持する構造体 //
    struct SectorInfo
    {
        float radius;       // 扇形の半径（視界距離） //
        float startAngle;   // 扇形の開始角度（ラジアン） //
        float endAngle;     // 扇形の終了角度（ラジアン） //

        SectorInfo()
            : radius(7.0f),startAngle(0),endAngle(0)  // デフォルト視界距離 7.0f //
        {}

        // 扇形の半径を設定
        // @param radius_ 設定する半径 //
        void SetRadius(float radius_) { radius = radius_; }
    };

    SectorInfo m_sectorInfo;  // 人間の扇形視界情報 //

};