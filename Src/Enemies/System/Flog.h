#pragma once
#include "../../Common/Object3D.h"
#include "../AnimalDog/ShepherdDog.h"
#include "../AnimalSheep/Sheep.h"

//鄒､繧後↓髢｢縺吶ｋ諠・ｱ
struct FlogInfo
{
    VECTOR3 centroid; //鄒､繧後・驥榊ｿ・
    CSheep* furthestSheep; //荳逡ｪ驕縺上↓縺・ｋ鄒翫・ptr;
    float maxDistance; //荳ｭ蠢・°繧我ｸ逡ｪ驕縺上↓縺・ｋ蜍慕黄縺ｾ縺ｧ縺ｮ霍晞屬
};

class CFlog : public Object3D
{
public:
    CFlog();
    ~CFlog();
    void Initialize();  // 鄒翫→迥ｬ繧堤函謌舌＠縺ｦ邏蝉ｻ倥￠
    FlogInfo CalcFlogInfo(const std::vector<CSheep*>& manySheep) const;
    const std::vector<CSheep*>& GetAllSheeps() const { return m_allSheep; }

    // 鄒､繧後・荳ｭ蠢・せ縺ｨ蜊雁ｾ・・蜿門ｾ・
    const VECTOR3& GetFlockCenter() const { return m_flockCenter; }
    float GetFlockRadius() const { return m_flockRadius; }

private:
    void RemoveFromArray(const CSheep* sheep);

    std::vector<CSheep*> m_allSheep;
    std::vector<CAShepherdDog*> m_shepherdDogs;

    // 鄒､繧後・荳ｭ蠢・せ縺ｨ蜊雁ｾ・
    VECTOR3 m_flockCenter;
    float m_flockRadius;
};

