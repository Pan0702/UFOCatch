#pragma once
#include "ComponentBase.h"
#include "Destroy.h"

#include "../System/AStarPathFinder.h"
class CAShepherdDog;
class CSheep;

class CShepherdDogWalk : public CComponentBase
{
public:
    CShepherdDogWalk(CAShepherdDog* dog, float speed);
    void Enter() override;
    void Update() override;

private:
    /// @brief 群れの外側リング（FlockRadius+MARGIN〜MoveRadius-MARGIN）からランダムな目標位置を決める
    /// @return Flock があれば true、無ければ false
    bool CalcRandomMoveInRing();

    /// @brief 次の位置が移動許容範囲内か判定する
    /// @brief 内側円（FlockRadius）に侵入していたり、外側のMoveAreaを超えていたら false。
    /// @param nextPos 次フレームに到達する予定の位置
    /// @return 移動可能なら true
    bool CanMoveTo(const VECTOR3& nextPos) const;

    /// @brief 指定位置が群れの内側円（FlockRadius）の内部にあるか
    /// @param nextPos チェック対象の位置
    /// @return 内側にある（侵入する）なら true、Flock が無いときは false
    bool IsInsideInnerCircle(const VECTOR3& nextPos) const;

    /// @brief 目標位置の方向へ滑らかに回転する
    /// @brief 目標までの距離が ARRIVAL_DISTANCE 未満なら m_isFinish=true にして false を返す。
    /// @param targetPos 向かいたい位置
    /// @return 回転を適用した場合 true、到着済みで終了した場合 false
    bool TryRotateToward(const VECTOR3& targetPos);

    /// @brief 移動先が内側円に入る場合、接線方向にスライドさせて回り込ませる
    /// @brief 中心方向への成分を除去して接線成分のみ残す。それでも内側に残るなら境界外へ押し出してクランプする。
    /// @param moveVec 補正前の移動ベクトル
    /// @return 内側円を侵さないように補正した移動ベクトル
    VECTOR3 SlideAlongInnerCircle(const VECTOR3& moveVec) const;

    CAShepherdDog* m_pOwner = nullptr;
    VECTOR3 m_targetPos = {};
    float m_moveSpeed = 0.0f;
};

class CCollecting : public CComponentBase
{
public:
    CCollecting(CAShepherdDog* dog, float speed);
    void Enter() override;
    void Update() override;

private:
    /// @brief 経路インデックスを次へ進める。直線的に並ぶ次ポイントはまとめてスキップ
    /// @brief 経路の最後に到達した場合は FinishAndHerdTarget() を呼ぶ。
    void AdvancePathIndex();

    /// @brief はぐれ羊を再選定し、目標位置と経路を作り直す
    /// @brief 失敗時（Flock 無し / はぐれ羊無し / 中心と一致）は m_isFinish=true。
    /// @brief 目標が既に十分近い場合は FinishAndHerdTarget() で完了。
    void RecomputePath();

    /// @brief 群れの内側円から最も離れたはぐれ羊を1匹選んで返す
    /// @return 一番遠いはぐれ羊。1匹もいなければ nullptr
    CSheep* FindFurthestStraySheep() const;

    /// @brief 対象羊の位置関係から、犬が向かうべき目標位置 m_targetPos を決める
    /// @brief 群れ中心からの距離が outsideThreshold 以内なら羊の手前へ、それより遠ければ背後へ回り込む位置にする。
    /// @param targetSheep 連れ戻したいはぐれ羊
    /// @return 計算できれば true、羊が群れ中心と重なっていて方向が決まらない場合は false
    bool CalcCollectTargetPos(CSheep* targetSheep);

    /// @brief 現在位置から m_targetPos までの経路を A* で計算し、m_path/m_pathIndex を更新する
    void BuildPath();

    /// @brief 対象羊を HERDED 状態にして自身を完了させる
    void FinishAndHerdTarget();

    CAShepherdDog* m_pOwner;
    VECTOR3 m_targetPos = {0, 0, 0};
    float m_moveSpeed;

    std::vector<VECTOR2> m_path;
    int m_pathIndex = 0;
    CAStarPathFinder m_pathFinder;
    float m_repathTimer = 0.0f;
    CSheep* m_targetSheep = nullptr;
    const float REPATH_INTERVAL = 0.2f;
};

class CDriving : public CComponentBase
{
public:
    CDriving(CAShepherdDog* dog, float speed);

    /// @brief UFOから群れを遠ざけるため、群れの向こう側に目標位置を決める
    void Enter() override;

    /// @brief 目標位置へ直進し、到着したら完了する
    void Update() override;

private:
    CAShepherdDog* m_pOwner;
    VECTOR3 m_targetPos = {0, 0, 0};
    VECTOR3 m_destination = {0, 0, 0};
    float m_moveSpeed;
};

class CRescue : public CComponentBase
{
public:
    CRescue(CAShepherdDog* dog);

    /// @brief 救出キューの先頭羊を対象にし、群れの重心を保存して救出を開始する
    void Enter() override;

    /// @brief 対象羊へ近づいた後、群れの重心方向へ誘導する
    void Update() override;

private:
    enum Phase : int8_t
    {
        APPROACH_SHEEP, // フェーズ1: 羊に近づく
        GUIDE_TO_CENTER // フェーズ2: 重心まで誘導
    };

    CAShepherdDog* m_pOwner;
    CSheep* m_targetSheep = nullptr; // 救出対象の羊
    VECTOR3 m_centroid = {0, 0, 0}; // 群れの重心
    Phase m_phase;
};

class CDestroyShepherdDog : public CDestroy
{
public:
    CDestroyShepherdDog(CAShepherdDog* dog, int score, float exp);
    void Enter() override;

private:
    CAShepherdDog* m_pDog;
};
