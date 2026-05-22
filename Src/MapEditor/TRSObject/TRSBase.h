#pragma once
#include "../../Common/Object3D.h"
#include "../MouseRay.h"
#include <d3d11.h>

/// <summary>Axis で扱う状態や種別を表す列挙型</summary>
enum class Axis : uint8_t { None, X, Y, Z };

/// <summary>ステージエディタで使う TRSBase の情報と処理をまとめる型</summary>
class TRSBase : public Object3D
{
protected:
    /// <summary>ステージエディタで使う Info の情報と処理をまとめる型</summary>
    struct Info
    {
        std::unique_ptr<CFbxMesh> mesh;
        std::unique_ptr<MeshCollider> coll;
    };

    Info xInfo;
    Info yInfo;
    Info zInfo;

private:
    ID3D11DepthStencilState* m_pDepthOffState = nullptr;

public:
    /// Position を設定する
    /// @param pos 座標
    virtual void SetPosition(const VECTOR3& pos);

    /// Scale By Camera を毎フレームの状態を更新する
    /// @param camPos 座標
    void UpdateScaleByCamera(const VECTOR3& camPos);

public:
    /// TRSBase を初期化する
    TRSBase();
    /// TRSBase の終了処理を行う
    ~TRSBase();

    /// 描画する
    virtual void Render();


    /// Ray Hit Test を返す
    /// @param ray 判定に使用するレイ
    /// @param length length に渡す値
    /// @return 処理結果
    Axis RayHitTest(const Ray& ray, float length = 1000.0f);
};
