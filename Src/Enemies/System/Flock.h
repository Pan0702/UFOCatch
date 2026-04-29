#pragma once
#include "../../Common/Object3D.h"
#include "../AnimalDog/ShepherdDog.h"
#include "../AnimalSheep/Sheep.h"

//鄒､繧後↓髢｢縺吶ｋ諠・ｱ
struct FlockInfo
{
    VECTOR3 centroid; //鄒､繧後・驥榊ｿ・
    CSheep* furthestSheep; //荳逡ｪ驕縺上↓縺・ｋ鄒翫・ptr;
    float maxDistance; //荳ｭ蠢・°繧我ｸ逡ｪ驕縺上↓縺・ｋ蜍慕黄縺ｾ縺ｧ縺ｮ霍晞屬
};

class CFlock : public Object3D
{
public:
    CFlock(const VECTOR3& center = VECTOR3(0, 0, 0), float radius = 4.0f, int sheepCount = 10);
    const std::vector<CSheep*>& GetAllSheeps() const { return m_allSheep; }

    // 鄒､繧後・荳ｭ蠢・せ縺ｨ蜊雁ｾ・・蜿門ｾ・
    const VECTOR3& GetFlockCenter() const { return m_flockCenter; }
    float GetFlockRadius() const { return m_flockRadius; }

    void AddSheep(CSheep* sheep);
    void RemoveSheep(const CSheep* sheep);

    void SetShepherdDog(CAShepherdDog* dog) { m_pShepherdDog = dog; }
    CAShepherdDog* GetShepherdDog() const { return m_pShepherdDog; }
    static FlockInfo CalCFlockInfoStatic(const std::vector<CSheep*>& manySheep);
    float GetMoveRadius() const;
    float GetCollectRadius() const;
    bool ContainPos(const VECTOR3& pos) const;
    bool ContainCollectArea(const VECTOR3& pos) const; // 回収開始円
    bool ContainMoveArea(const VECTOR3& pos) const;

private:
    std::vector<CSheep*> m_allSheep;
    CAShepherdDog* m_pShepherdDog = nullptr;

    // 鄒､繧後・荳ｭ蠢・せ縺ｨ蜊雁ｾ・
    VECTOR3 m_flockCenter;
    float m_flockRadius;
};
