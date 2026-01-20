#pragma once
#include <chrono>
#include "../Base/StateBase.h"
#include "../../Common/Object3D.h"
#include "../../Player/Player.h"
#include "../Liner4Tree.h"

class CEnemyBase;

class CAnimalManager : public Object3D
{
public:
    CAnimalManager(int time);
    ~CAnimalManager();


    void SetRotationY(const float& angle);
    CFbxMesh* MeshList(const std::string& str);

    // 四分木から周辺のエネミーを取得
    std::vector<CEnemyBase*> GetNearbyEnemies(CEnemyBase* pObj, const VECTOR2& pos, const VECTOR2& size) const;

public:
    CPlayer* m_pPlayer;

private:
    void Update() override;
    VECTOR4 TimeColor(int time);

    struct meshstruct
    {
        std::string name;
        CFbxMesh* mesh;
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
