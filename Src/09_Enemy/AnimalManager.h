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
            if (m_pCubes[i] == obj)
            {
                m_pCubes[i]->~CACube(); // デストラクタ呼び出し
                Cube.Free(m_pCubes[i]);
                m_pCubes[i] = nullptr;
                break; // 見つけたら終了
            }
        }
    }
    bool CubesFree();
    //各座標の最大値を返却
    VECTOR3 GetObjectSize(MeshCollider* meshColl) const;
    CPlayer* m_pPlayer;

private:
    void Update() override;
    void Draw() override;
    
    std::vector<CACube*> m_pCubes;
    PoolAllocator<CACube, 100> Cube;
};
