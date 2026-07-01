#pragma once
#include "../Common/Object3D.h"

/// <summary>ステージで使う Sky Box の情報と処理をまとめる型</summary>
class CSkyBox : public Object3D
{
public:
    /// CSkyBox を初期化する
    /// @param meshPath パス
    CSkyBox(const char* meshPath);
    /// CSkyBox の終了処理を行う
    ~CSkyBox();

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

    ID3D11RasterizerState* m_pRStateCullNone;
};
