#pragma once
#include "../Common/Object3D.h"
#include "Player.h"
#include "PlayerLevel.h"

/// <summary>プレイヤーで使う Sprite Image の情報と処理をまとめる型</summary>
class CSpriteImage;

/// <summary>プレイヤーで使う Cone Draw の情報と処理をまとめる型</summary>
class CConeDraw : public Object3D
{
public:
    /// CConeDraw を初期化する
    /// @param coneTopPos 座標
    CConeDraw(float coneTopPos);
    /// CConeDraw を初期化する
    /// @param coneTopPos 座標
    /// @param p p に渡す値
    CConeDraw(float coneTopPos, CPlayer* p);
    /// CConeDraw の終了処理を行う
    ~CConeDraw() override;

private:
    /// 開始する
    void Start() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

    CPlayer* m_pPlayer;
    CPlayerLevel* m_pLevel;
};

/// <summary>プレイヤーで使う Circle Draw の情報と処理をまとめる型</summary>
class CCircleDraw : public Object3D
{
public:
    /// CCircleDraw を初期化する
    CCircleDraw();
    /// CCircleDraw を初期化する
    /// @param p p に渡す値
    CCircleDraw(CPlayer* p);
    /// CCircleDraw の終了処理を行う
    ~CCircleDraw() override;

private:
    /// 開始する
    void Start() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

    CPlayer* m_pPlayer;
    std::unique_ptr<CSpriteImage> m_pCircleImage;
    CPlayerLevel* m_pLevel;

    float m_radius;
};
