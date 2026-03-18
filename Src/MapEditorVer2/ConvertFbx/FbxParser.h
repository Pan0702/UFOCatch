#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../Utils/MyMath.h"

// .mesh 繝輔ぃ繧､繝ｫ縺ｮ鬆らせ讒矩菴難ｼ・tride = 32 bytes・・
struct MeshVertex
{
    float position[3];  // 12 bytes
    float normal[3];    // 12 bytes
    float uv[2];        //  8 bytes
};

// FBX Binary 繝輔ぃ繧､繝ｫ繧定ｧ｣譫舌＠縺ｦ鬆らせ繝ｻ繧､繝ｳ繝・ャ繧ｯ繧ｹ繝・・繧ｿ繧貞叙蠕励☆繧九け繝ｩ繧ｹ
class FbxParser
{
private:
    // FBX 繝弱・繝画ュ蝣ｱ
    struct Node
    {
        std::string name;
        uint64_t    propStart  = 0;
        uint64_t    propLen    = 0;
        uint64_t    childStart = 0;
        uint64_t    end         = 0;
        std::vector<Node> children;
    };

    std::vector<uint8_t> m_data;
    uint32_t             m_version = 0;
    bool                 m_is64bit = false;
    std::vector<Node>    m_roots;
    // Connections 縺九ｉ讒狗ｯ峨☆繧九Ν繝・け繧｢繝・・繝・・繝悶Ν
    std::unordered_map<int64_t, Node*>   m_modelMap;          // Model ID 竊・Node*
    std::unordered_map<int64_t, int64_t> m_modelParentMap;   // Model ID 竊・隕ｪ Model ID

public:
    /// <summary>
    /// FBX 繝舌う繝翫Μ繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ繧
    /// </summary>
    /// <param name="path">FBX 繝輔ぃ繧､繝ｫ縺ｮ繝代せ</param>
    /// <returns>謌仙粥縺ｧ true</returns>
    bool Load(const std::string& path);

    /// <summary>
    /// 隱ｭ縺ｿ霎ｼ繧薙□ FBX 縺九ｉ鬆らせ繝ｻ繧､繝ｳ繝・ャ繧ｯ繧ｹ繝・・繧ｿ繧貞ｱ暮幕縺吶ｋ縲・
    /// Objects 逶ｴ荳九・蜈ｨ Geometry 繧堤ｵ仙粋縺励※1縺､縺ｮ繝｡繝・す繝･縺ｨ縺励※蜃ｺ蜉帙☆繧九・
    /// 蜷・Geometry 縺ｫ蟇ｾ蠢懊☆繧・Model 縺ｮ繝ｭ繝ｼ繧ｫ繝ｫ Transform (T/R/S) 繧帝←逕ｨ縺吶ｋ縲・
    /// </summary>
    /// <param name="outVerts">蜃ｺ蜉幃らせ驟榊・</param>
    /// <param name="outIndices">蜃ｺ蜉帙う繝ｳ繝・ャ繧ｯ繧ｹ驟榊・</param>
    /// <returns>謌仙粥縺ｧ true</returns>
    bool ExtractMesh(
        std::vector<MeshVertex>& outVerts,
        std::vector<uint32_t>&   outIndices);
    
    // FBX 縺ｫ蝓九ａ霎ｼ縺ｾ繧後◆繝・け繧ｹ繝√Ε縺ｮ繝輔ぃ繧､繝ｫ蜷阪ｒ蜿門ｾ励☆繧・
    // RelativeFilename 縺檎ｩｺ縺ｪ繧・FileName 縺ｮ繝輔ぃ繧､繝ｫ蜷埼Κ蛻・ｒ霑斐☆
    std::string GetTextureFileName() const;

private:
    /// <summary>
    /// 1縺､縺ｮ Geometry 繝弱・繝峨ｒ繝ｭ繝ｼ繧ｫ繝ｫ Transform 繧帝←逕ｨ縺励↑縺後ｉ螻暮幕縺吶ｋ縲・
    /// out_verts / out_indices 縺ｮ譛ｫ蟆ｾ縺ｫ霑ｽ險倥☆繧九・
    /// </summary>
    /// <param name="geometry">螻暮幕縺吶ｋ Geometry 繝弱・繝・/param>
    /// <param name="model">蟇ｾ蠢懊☆繧・Model 繝弱・繝会ｼ・ullptr 縺ｮ蝣ｴ蜷医・ Transform 縺ｪ縺暦ｼ・/param>
    /// <param name="outVerts">霑ｽ險伜・縺ｮ鬆らせ驟榊・</param>
    /// <param name="outIndices">霑ｽ險伜・縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ驟榊・</param>
    /// <returns>謌仙粥縺ｧ true縲７ertices 縺ｮ縺ｪ縺・Geometry 縺ｯ繧ｹ繧ｭ繝・・縺励※ true 繧定ｿ斐☆</returns>
    bool ExtractGeometry(
        Node*                    geometry,
        Node*                    model,
        std::vector<MeshVertex>& outVerts,
        std::vector<uint32_t>&   outIndices);

    /// <summary>
    /// 繝弱・繝峨・譛蛻昴・繝励Ο繝代ユ繧｣・・D・峨ｒ int64_t 縺ｧ隱ｭ繧縲・
    /// FBX 7400 莉･蜑阪・ 'I'(int32)縲・500 莉･髯阪・ 'L'(int64)縲・
    /// </summary>
    int64_t ReadNodeId(uint64_t offset) const;

    /// <summary>
    /// Model 繝弱・繝峨°繧峨げ繝ｭ繝ｼ繝舌Ν螟画鋤陦悟・ (隕ｪ繝√ぉ繝ｼ繝ｳ繧貞・蟶ｰ逧・↓邨仙粋) 繧貞叙蠕励☆繧九・
    /// 陦悟━蜈・4x4縲・uler 蝗櫁ｻ｢鬆・ｺ上・ FBX 讓呎ｺ悶・ XYZ縲・
    /// model 縺・nullptr 縺ｮ蝣ｴ蜷医・蜊倅ｽ崎｡悟・繧定ｿ斐☆縲・
    /// </summary>
    MATRIX4X4 GetGlobalMatrix(Node* model) const;

    /// <summary>
    /// Model 繝弱・繝峨°繧峨Ο繝ｼ繧ｫ繝ｫ螟画鋤陦悟・ (TRS) 繧貞叙蠕励☆繧九・
    /// 陦悟━蜈・4x4縲・uler 蝗櫁ｻ｢鬆・ｺ上・ FBX 讓呎ｺ悶・ XYZ縲・
    /// model 縺・nullptr 縺ｮ蝣ｴ蜷医・蜊倅ｽ崎｡悟・繧定ｿ斐☆縲・
    /// </summary>
    MATRIX4X4 GetLocalMatrix(Node* model) const;

    // 繝舌う繝翫Μ繝・・繧ｿ縺九ｉ蝙・T 縺ｮ蛟､繧定ｪｭ縺ｿ霎ｼ繧
    template<typename T>
    T Read(uint64_t offset) const
    {
        T v{};
        memcpy(&v, m_data.data() + offset, sizeof(T));
        return v;
    }

    // 蝙倶ｻ倥″驟榊・繝励Ο繝代ユ繧｣繧定ｪｭ縺ｿ霎ｼ繧・磯撼蝨ｧ邵ｮ / zlib 蝨ｧ邵ｮ縺ｮ荳｡譁ｹ縺ｫ蟇ｾ蠢懶ｼ・
    // 螳溯｣・・ FbxParser.cpp 縺ｫ險倩ｿｰ・・ouble / int32_t 縺ｮ縺ｿ菴ｿ逕ｨ蜿ｯ・・
    template<typename T>
    std::vector<T> ReadTypedArray(uint64_t offset);

    Node              ReadNodeHeader(uint64_t offset, uint64_t& outEnd) const;
    std::vector<Node> ParseChildren(uint64_t start, uint64_t end);
    static Node*             FindNode(std::vector<Node>& nodes, const std::string& name);

    /// <summary>謖・ｮ壹ヮ繝ｼ繝峨・蟄舌°繧・double 驟榊・繝励Ο繝代ユ繧｣繧定ｪｭ縺ｿ霎ｼ繧</summary>
    std::vector<double>  ReadDoubleArray(Node* parent, const std::string& nodeName);

    /// <summary>謖・ｮ壹ヮ繝ｼ繝峨・蟄舌°繧・int32 驟榊・繝励Ο繝代ユ繧｣繧定ｪｭ縺ｿ霎ｼ繧</summary>
    std::vector<int32_t> ReadIntArray(Node* parent, const std::string& nodeName);

    /// <summary>謖・ｮ壹ヮ繝ｼ繝峨・蟄舌°繧画枚蟄怜・繝励Ο繝代ユ繧｣繧定ｪｭ縺ｿ霎ｼ繧</summary>
    std::string          ReadString(Node* parent, const std::string& nodeName) const;
};
