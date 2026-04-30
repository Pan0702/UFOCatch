#pragma once
#include "../../Common/Object3D.h"
#include "../AnimalDog/ShepherdDog.h"
#include "../AnimalSheep/Sheep.h"

//群れに関する集計情報
struct FlockInfo
{
    VECTOR3 centroid; // 群れの重心
    CSheep* furthestSheep; // 重心から一番遠い羊
    float maxDistance; // 重心から一番遠い羊までの距離の二乗
};

class CFlock : public Object3D
{
public:
    CFlock(const VECTOR3& center = VECTOR3(0, 0, 0), float radius = 4.0f, int sheepCount = 10);
    const std::vector<CSheep*>& GetAllSheeps() const { return m_allSheep; }

    /// @brief  群れの中心と半径を取得
    const VECTOR3& GetFlockCenter() const { return m_flockCenter; }
    float GetFlockRadius() const { return m_flockRadius; }

    /// @brief 羊を追加
    void AddSheep(CSheep* sheep);
    /// @brief 羊を削除
    void RemoveSheep(const CSheep* sheep);
    /// @brief 群れの犬を設定
    void SetShepherdDog(CAShepherdDog* dog) { m_pShepherdDog = dog; }
    /// @brief 群れの犬を取得
    CAShepherdDog* GetShepherdDog() const { return m_pShepherdDog; }

    /// @brief 複数の羊から重心と、重心から最も遠い羊を計算する
    /// @param manySheep 集計対象の羊リスト
    /// @return 重心、最遠羊、最遠距離の二乗を含む情報
    static FlockInfo CalCFlockInfoStatic(const std::vector<CSheep*>& manySheep);

    /// @brief 羊と犬が通常移動できる外側円の半径を取得する
    float GetMoveRadius() const;

    /// @brief 回収開始判定に使う円の半径を取得する
    float GetCollectRadius() const;

    /// @brief 指定位置が群れの内側円に含まれるか判定する
    bool ContainPos(const VECTOR3& pos) const;

    /// @brief 指定位置が回収開始円に含まれるか判定する
    bool ContainCollectArea(const VECTOR3& pos) const;

    /// @brief 指定位置が移動可能な外側円に含まれるか判定する
    bool ContainMoveArea(const VECTOR3& pos) const;

private:
    std::vector<CSheep*> m_allSheep;
    CAShepherdDog* m_pShepherdDog = nullptr;

    // 群れの中心と半径
    VECTOR3 m_flockCenter;
    float m_flockRadius;
};
