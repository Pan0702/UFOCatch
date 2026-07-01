#pragma once
#include "../Common/Object3D.h"

// チュートリアルのUI表示を管理するクラス //
class CTutorialDisplayInfo : public Object3D
{
public:
    /// CTutorialDisplayInfo を初期化する
    CTutorialDisplayInfo();
    /// CTutorialDisplayInfo の終了処理を行う
    ~CTutorialDisplayInfo();

    // 表示タイプを設定する
    // @param type 表示タイプ //
    /// Display Type を設定する
    /// @param type type に渡す値
    void SetDisplayType(int type) { m_nDisplayType = type; }

    // クエスト番号を設定する
    // @param quest クエスト番号 //
    /// Quest を設定する
    /// @param quest quest に渡す値
    void SetQuest(int quest) { m_questNum = quest; }

private:
    /// 開始する
    void Start() override;
    /// 描画する
    void Draw() override;

    // 操作説明UIを描画する //
    /// Operation Draw の処理を行う
    void OperationDraw() const;

    // 目標表示UIを描画する //
    /// Target Draw の処理を行う
    void TargetDraw() const;

    // 表示タイプ定数 //
    enum
    {
        Move = 0, // 移動操作 //
        Suction = 1, // 吸い込み操作 //
    };

    int m_nDisplayType; // 現在の表示タイプ //
    int m_questNum; // 現在のクエスト番号 //
    std::unique_ptr<CSpriteImage> m_pImage; // UI画像 //
};
