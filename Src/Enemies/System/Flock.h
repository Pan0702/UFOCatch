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

/// <summary>敵AIで使う Flock の情報と処理をまとめる型</summary>
class CFlock : public Object3D
{
public:
    /// CFlock を初期化する
    /// @param center center に渡す値
    /// @param radius 半径
    /// @param sheepCount 個数
    CFlock(const VECTOR3& center = VECTOR3(0, 0, 0), float radius = 4.0f, int sheepCount = 10);
    /// All Sheeps を取得する
    /// @return 取得した要素一覧
    const std::vector<CSheep*>& GetAllSheeps() const { return m_allSheep; }

    /// Flock Center を取得する
    /// @return 3次元ベクトル
    const VECTOR3& GetFlockCenter() const { return m_flockCenter; }
    /// Flock Radius を取得する
    /// @return 計算結果の値
    float GetFlockRadius() const { return m_flockRadius; }

    /// Sheep を追加する
    /// @param sheep sheep に渡す値
    void AddSheep(CSheep* sheep);
    /// Sheep を削除する
    /// @param sheep sheep に渡す値
    void RemoveSheep(const CSheep* sheep);
    /// Shepherd Dog を設定する
    /// @param dog dog に渡す値
    void SetShepherdDog(CAShepherdDog* dog) { m_pShepherdDog = dog; }
    /// Shepherd Dog を取得する
    /// @return 対象のポインタ
    CAShepherdDog* GetShepherdDog() const { return m_pShepherdDog; }

    /// Cal CFlock Info Static を返す
    /// @param manySheep manySheep に渡す値
    /// @return 処理結果
    static FlockInfo CalCFlockInfoStatic(const std::vector<CSheep*>& manySheep);

    /// Move Radius を取得する
    /// @return 計算結果の値
    float GetMoveRadius() const;

    /// Collect Radius を取得する
    /// @return 計算結果の値
    float GetCollectRadius() const;

    /// Contain Pos を返す
    /// @param pos 座標
    /// @return 成功または条件を満たす場合 true
    bool ContainPos(const VECTOR3& pos) const;

    /// Contain Collect Area を返す
    /// @param pos 座標
    /// @return 成功または条件を満たす場合 true
    bool ContainCollectArea(const VECTOR3& pos) const;

    /// Contain Move Area を返す
    /// @param pos 座標
    /// @return 成功または条件を満たす場合 true
    bool ContainMoveArea(const VECTOR3& pos) const;

private:
    std::vector<CSheep*> m_allSheep;
    CAShepherdDog* m_pShepherdDog = nullptr;

    // 群れの中心と半径
    VECTOR3 m_flockCenter;
    float m_flockRadius;
};
