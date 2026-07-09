#pragma once
#include "../Common/Object3D.h"
#include "../Utils/Sprite3D.h"
#include "../Utils/LerpValue.h"

// ゲーム中のUIを表示するクラス //
class CDisplayInfo : public Object3D
{
public:
    /// CDisplayInfo を初期化する
    CDisplayInfo();
    /// CDisplayInfo の終了処理を行う
    ~CDisplayInfo();
    /// 開始する
    void Start() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

private:
    /// Giwaku Draw の処理を行う
    void GiwakuDraw();
    /// Exp Draw の処理を行う
    void ExpDraw();
    /// Time Draw の処理を行う
    void TimeDraw();
    /// HPDraw の処理を行う
    void HPDraw();
    /// Lv Draw の処理を行う
    void LvDraw();
    /// Cut In の処理を行う
    /// @param num num に渡す値
    void CutIn(int num);

    CSprite* m_pSprite;
    CSpriteImage* m_playUIImage;
    CSpriteImage* m_expImage;
    CSpriteImage* m_giwakuImage;
    CSpriteImage* m_pLogo;

    // 経験値ゲージ用
    LerpValue m_xpWeightLerp;
    float m_prevProportion;
    float m_currentWidth;

    // 疑惑ゲージ用（円形）
    LerpValue m_giwakuAngleLerp;
    float m_prevGiwakuProportion;
    float m_currentAngle;

    int m_cutInCnt;
    bool m_isCutInDraw;
};
