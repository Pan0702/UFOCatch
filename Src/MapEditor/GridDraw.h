#pragma once
#include "../Common/Object3D.h"

/// <summary>ステージエディタで使う Grid Draw の情報と処理をまとめる型</summary>
class GridDraw : public Object3D
{
private:
    bool is_grid_draw_ = false;

private:
    /// 描画する
    void Draw() override;

    /// Grid の処理を行う
    static void Grid();

public:
    /// Draw Flag を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetDrawFlag() const { return is_grid_draw_; }
    /// Draw Flag を設定する
    /// @param flag flag に渡す値
    void SetDrawFlag(bool flag) { is_grid_draw_ = flag; }
};
