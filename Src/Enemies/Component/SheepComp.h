#pragma once
#include "ComponentBase.h"
#include "../AnimalSheep/Sheep.h"

class CHerded : public CComponentBase
{
public:
    CHerded(CSheep* sheep);
    void Enter() override;
    void Update() override;

private:
    /// @brief 羊が群れの中心寄りまで戻ったか判定する
    bool IsDeepInsideFlock() const;

    /// @brief 合成された力の方向へ回転しながら移動する
    /// @param force 群れに戻る力やBoidsの合計ベクトル
    void ApplyMovement(const VECTOR3& force);

    /// @brief 仲間への凝集と近すぎる仲間からの分離を計算する
    /// @return Boidsで得た移動方向の力
    VECTOR3 CalcBoids() const;

    /// @brief 牧羊犬が近い場合に犬から逃げる力を計算する
    /// @return 犬から離れる方向の力。犬がいない場合はゼロベクトル
    VECTOR3 CalcEscapeFromDog() const;

    /// @brief 一定間隔でランダム方向を変えながら徘徊する力を計算する
    /// @return 徘徊方向の力
    VECTOR3 CalcWandering(); // ランダムな徘徊行動

    /// @brief 群れの外側または端にいる羊を中心へ戻す力を計算する
    /// @return 群れ中心へ戻る力。十分内側ならゼロベクトル
    VECTOR3 CalcRetrunToFlock();


    CSheep* m_pOwner;

    // Wandering用のパラメータ
    VECTOR3 m_wanderTarget;
    float m_wanderTimer;

    // 移動時間管理（一定時間後にIDLEに戻る）
    float m_walkDuration;
    float m_walkTimer;

    // 回転の滑らかさ
    float m_currentRotation;
};


class CPanic : public CComponentBase
{
public:
    CPanic(CSheep* sheep);
    void Enter() override;
    void Update() override;

private:
    /// @brief 一定時間ごとにパニック移動の方向をランダムに更新する
    void UpdateDirection();

    /// @brief パニック方向へ向きを合わせて移動する
    void UpdateMovement() const;

    /// @brief パニック時間終了後、所属する群れの有無でHERDEDまたはIDLEへ遷移する
    void CheckBoundaryAndTransition();

    CSheep* m_pOwner;
    VECTOR3 m_panicDirection; // ランダムな逃げる方向
    float m_changeDirectionTimer = 0.0f;
    float m_panicDuration = 0.0f;
    float m_panicTimer = 0.0f;
    VECTOR3 m_startPos;
};
