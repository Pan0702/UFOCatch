#pragma once
#include <chrono>

#include "ACube.h"
#include "../05_CommonFile/Object3D.h"
#include "../08_Player/Player.h"
#include "../04_FrameWork/PoolAllocator.h"

class CAnimalManager : public Object3D
{
public:
    CAnimalManager();
    ~CAnimalManager();

    template <class T>
    void Destroy(T* obj)
    {
        for (int i = 0; i < 100; i++)
        {
            if (pCubes[i] == obj)
            {
                //pCubes[i]->~CACube(); // デストラクタ呼び出し
                Cube.Free(pCubes[i]);
                break; // 見つけたら終了
            }
        }
    }

protected:
    //各座標の最大値を返却
    VECTOR3 GetObjectSize(MeshCollider* meshColl) const;
    CPlayer* m_pPlayer;

private:
    void Update() override;
    void Draw() override;
    CACube* pCubes[100] = {nullptr}; ;
    PoolAllocator<CACube, 100> Cube;
};
