#pragma once
#include "../Common/Object3D.h"

// チュートリアルのUI表示を管理するクラス //
class CTutorialDisplayInfo : public Object3D
{
public:
    CTutorialDisplayInfo();

    // 表示タイプを設定する
    // @param type 表示タイプ //
    void SetDisplayType(int type){ m_nDisplayType = type; }

    // クエスト番号を設定する
    // @param quest クエスト番号 //
    void SetQuest(int quest){ m_questNum = quest; }

private:
    ~CTutorialDisplayInfo();
    void Draw() override;

    // 操作説明UIを描画する //
    void OperationDraw() const;

    // 目標表示UIを描画する //
    void TargetDraw();

    // 表示タイプ定数 //
    enum
    {
        Move = 0,     // 移動操作 //
        Suction = 1,  // 吸い込み操作 //
    };

    int m_nDisplayType;       // 現在の表示タイプ //
    int m_questNum;           // 現在のクエスト番号 //
    CSprite* m_pSprite;       // スプライト描画オブジェクト //
    CSpriteImage* m_pImage;   // UI画像 //
};
