#pragma once
#include "../../Common/Object3D.h"
#include "../MouseRay.h"
#include <d3d11.h>

enum class Axis { None, X, Y, Z };

class TRSBase : public Object3D
{
protected:
    struct Info
    {
        std::unique_ptr<CFbxMesh> mesh_;
        std::unique_ptr<MeshCollider> coll_;
    };

    Info x_info_;
    Info y_info_;
    Info z_info_;

private:
    ID3D11DepthStencilState* depth_off_state_ = nullptr;

public:
    /// <summary>ギズモの表示位置を指定座標に設定する</summary>
    /// <param name="pos">設定するワールド座標</param>
    virtual void SetPosition(const VECTOR3& pos);

    /// <summary>カメラ距離に比例してギズモのスケールを更新し、常に同じ見かけサイズにする</summary>
    /// <param name="cam_pos">カメラのワールド座標</param>
    void UpdateScaleByCamera(const VECTOR3& cam_pos);

public:
    TRSBase();
    ~TRSBase();

    /// <summary>X/Y/Z軸のギズモメッシュをすべて描画する</summary>
    virtual void Render();
    

    /// <summary>レイとX/Y/Zギズモのコライダーを判定し、当たった軸を返す</summary>
    /// <param name="ray">判定に使用するレイ</param>
    /// <param name="length">レイの長さ（デフォルト1000）</param>
    /// <returns>当たった軸（X/Y/Z）、当たらなければNone</returns>
    Axis RayHitTest(const Ray& ray, float length = 1000.0f);
};
