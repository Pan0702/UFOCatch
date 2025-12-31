#pragma once
#include <chrono>
#include "../Base/StateBase.h"
#include "../../05_CommonFile/Object3D.h"
#include "../../08_Player/Player.h"
#include "../Liner4Tree.h"

class CEnemyBase;

class CAnimalManager : public Object3D
{
public:
    CAnimalManager();
    ~CAnimalManager();

    //各座標の最大値を返却
    VECTOR3 GetObjectCenter(const CFbxMesh* fbxMesh) const;
    virtual void  SetRotationY(const float& angle);
    CFbxMesh* MeshList(const std::string& str);
public:
     CPlayer* m_pPlayer;

private:
    void Update() override;

    struct meshstruct{
        std::string name;
        CFbxMesh* mesh;
        VECTOR3 center;
    };
    std::list<meshstruct> m_meshList;
    CLiner4Tree<CEnemyBase>* m_pTree;


};

enum AnimationType 
{
    A_IDEL = 0,
    A_WALK,
    A_RUN,
    A_SEACH
};