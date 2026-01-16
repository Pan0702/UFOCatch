#pragma once
#include "../05_CommonFile/Object3D.h"
#include "../06_GameLib/LerpValue.h"

// プレイヤーを追従するカメラクラス //
class CPlayerCamera:public Object3D
{
public:
    CPlayerCamera();
    ~CPlayerCamera();
    // カメラ位置を設定する
    // @param pos プレイヤーの位置
    // @param distance コーンの高さ //
    void PosSet(const VECTOR3& pos, const float& distance);
    // カメラをズームインさせる
    // @param pos プレイヤーの位置 //
    void ZoomIn(const VECTOR3& pos);
    // カメラをズームアウトさせる
    // @param pos プレイヤーの位置 //
    void ZoomOut(const VECTOR3& pos);
private:
    void Update() override;
    void UpdateCameraBezier();


    VECTOR3 m_camPos;
    VECTOR3 m_camLook;
    VECTOR3 t1;
    VECTOR3 t2;
    BezierValueVec3 m_camPosBezier;
    BezierValueVec3 m_camLookBezier;
    
};
