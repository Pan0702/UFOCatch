#pragma once
#include "MouseRay.h"
#include "../Framework/ResourceManager.h"
#include "../Common/Object3D.h"

struct StageDataInfo
{
    Transform transform;
    std::string modelName;
    StageDataInfo()
    {
        transform.position = VECTOR3(0, 0, 0);
        modelName = "";
    }
    StageDataInfo(const std::string& model_name,const VECTOR3& pos)
    {
        modelName = model_name;
        transform.position = pos;
    }
    StageDataInfo(const std::string& model_name,const Transform& t)
    {
        modelName = std::move(model_name);
        transform = t;
    }
};

class StageData : public Object3D
{
private:

    std::vector<StageDataInfo> m_stageData;
    * model_storage_;
    int m_selectedModel = -1;
private:
    void Draw () override;
public:
    StageData();
    ~StageData() = default;

    /// <summary>謖・ｮ壼ｺｧ讓吶→繝｢繝・Ν蜷阪〒繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺ｫ霑ｽ蜉縺吶ｋ</summary>
    /// <param name="pos">驟咲ｽｮ縺吶ｋ繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・/param>
    /// <param name="modelName">菴ｿ逕ｨ縺吶ｋ繝｢繝・Ν縺ｮ蜷榊燕</param>
    void AddModel(const VECTOR3& pos, const std::string& modelName);
    
    void AddModel(const Transform& t ,const std::string&modelName);
    
    /// <summary>Transform  蜈ｨ菴薙ｒ謖・ｮ壹＠縺ｦ繧ｪ繝悶ず繧ｧ繧ｯ繝・繧偵せ繝・・繧ｸ縺ｫ霑ｽ蜉縺吶ｋ・医う繝ｳ繝昴・繝育畑・・/summary>               
    /// <param name="modelName">菴ｿ逕ｨ縺吶ｋ 繝｢繝・Ν縺ｮ蜷榊燕</param>        
    /// <param name="transform">驟咲ｽｮ縺吶ｋTransform・井ｽ咲ｽｮ繝ｻ蝗櫁ｻ｢繝ｻ繧ｹ繧ｱ繝ｼ繝ｫ・・/param>  
    void AddModelWithTransform(const std::string& modelName, const Transform& transform);

    /// <summary>繧ｹ繝・・繧ｸ繝・・繧ｿ繧谷SON繝輔ぃ繧､繝ｫ縺ｫ繧ｨ繧ｯ繧ｹ繝昴・繝医☆繧・/summary>
    /// <param name="filename">蜃ｺ蜉帙ヵ繧｡繧､繝ｫ蜷搾ｼ域僑蠑ｵ蟄舌↑縺暦ｼ・/param>
    void Export(const std::string& filename) const;

    /// <summary>繝ｬ繧､縺ｨ繧ｹ繝・・繧ｸ荳翫・蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝｡繝・す繝･繧ｳ繝ｩ繧､繝繝ｼ繧貞愛螳壹＠縲∵怙霑第磁縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ繧定ｿ斐☆</summary>
    /// <param name="ray">蛻､螳壹↓菴ｿ逕ｨ縺吶ｋ繝ｬ繧､</param>
    /// <param name="collOut">陦晉ｪ∵ュ蝣ｱ縺ｮ蜃ｺ蜉帛・</param>
    /// <returns>蠖薙◆縺｣縺溘が繝悶ず繧ｧ繧ｯ繝医・繧､繝ｳ繝・ャ繧ｯ繧ｹ縲ょｽ薙◆繧峨↑縺代ｌ縺ｰ-1</returns>
    int RayHitTest(const Ray& ray, MeshCollider::CollInfo* collOut);

    /// <summary>迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺九ｉ蜑企勁縺吶ｋ</summary>
    void DeleteModel();
    
    /// <summary>謖・ｮ壹う繝ｳ繝・ャ繧ｯ繧ｹ縺ｫ縺ゅｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺九ｉ蜑企勁縺吶ｋ</summary>
    /// /// <param name="index">謖・ｮ壹う繝ｳ繝・ャ繧ｯ繧ｹ</param>
    void DeleteModel(int index);

    /// <summary>迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繝昴う繝ｳ繧ｿ繧定ｿ斐☆縲よ悴驕ｸ謚樊凾縺ｯnullptr</summary>
    Transform* GetSelectedTransform();

    /// <summary>謖・ｮ壹う繝ｳ繝・ャ繧ｯ繧ｹ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繧剃ｸ頑嶌縺阪☆繧具ｼ・ndo/Redo逕ｨ・・/summary>
    /// <param name="index">蟇ｾ雎｡繧､繝ｳ繝・ャ繧ｯ繧ｹ</param>
    /// <param name="transform">險ｭ螳壹☆繧亀ransform</param>
    void SetSelectedTransform(int index, const Transform& transform);

    /// <summary>驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医う繝ｳ繝・ャ繧ｯ繧ｹ繧定ｨｭ螳壹☆繧・/summary>
    /// <param name="index">驕ｸ謚槭☆繧九う繝ｳ繝・ャ繧ｯ繧ｹ</param>
    void SetModel(int index);

    /// <summary>謖・ｮ壹う繝ｳ繝・ャ繧ｯ繧ｹ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺ｫ繧ｳ繝斐・縺吶ｋ</summary>
    /// <param name="index"> 繧ｳ繝斐・縺吶ｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医・驟榊・逡ｪ蜿ｷ縲</param>
    void CopyModel(int index);
    
    /// <summary>迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医う繝ｳ繝・ャ繧ｯ繧ｹ繧定ｿ斐☆</summary>
    int GetSelectIndex() const;

    /// <summary>繧ｹ繝・・繧ｸ縺ｫ險ｭ鄂ｮ縺輔ｌ縺ｦ繧九☆縺ｹ縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑斐☆</summary>
    const std::vector<StageDataInfo>& GetStageDataInfo() const;
};



