#pragma once
#include <vector>
#include "../Utils/MyMath.h"

#include "QuadTreeCell.h"

template <typename T>
class CLiner4Tree
{
public:
    CLiner4Tree(int level, const VECTOR4& area_)
        : m_area(area_), m_maxLevel(level)
    {
        int size = ((1 << ((level + 1) * 2)) - 1) / 3;
        m_cells.resize(size);
    }
    //隕∫ｴ繧貞炎髯､
    void AllClear() { for (auto& cell : m_cells) cell.Clear(); }

    /// 繧ｪ繝悶ず繧ｧ繧ｯ繝医・霑ｽ蜉
    /// @param obj 繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繝・
    /// @param pos XZ蟷ｳ髱｢縺ｧ縺ｮ蝣ｴ謇
    /// @param size 繧ｪ繝悶ず繧ｧ繧ｯ繝医・螟ｧ縺阪＆
    /// @return 繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑ｽ蜉縺ｧ縺阪◆繧欝rue///
    bool Register(T* obj, const VECTOR2& pos, const VECTOR2& size)
    {
        // 繧ｪ繝悶ず繧ｧ繧ｯ繝医′螻槭☆繧九そ繝ｫ逡ｪ蜿ｷ繧貞叙蠕・
        uint16_t cellIndex = GetMortonNumber(pos, size);

        if (cellIndex >= m_cells.size()) return false;

        // 繧ｻ繝ｫ縺ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑ｽ蜉
        m_cells[cellIndex].Push(obj);

        return true; 
    }

    ///
    /// @param obj 繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繝・
    /// @param pos XZ蟷ｳ髱｢縺ｧ縺ｮ蝣ｴ謇
    /// @param size 繧ｪ繝悶ず繧ｧ繧ｯ繝医・螟ｧ縺阪＆
    /// @return  霑代￥縺ｫ縺・ｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑泌唆//
    std::vector<T*> GetObjects(T* pObj, const VECTOR2& pos, const VECTOR2& size)
    {
        std::vector<T*> collisionList;
        // 繧ｪ繝悶ず繧ｧ繧ｯ繝医・蟾ｦ荳翫→蜿ｳ荳九・繝｢繝ｼ繝医Φ逡ｪ蜿ｷ
        uint16_t cellIndex = GetMortonNumber(pos, size);
        
        int currentLevel = GetLevelFromIndex(cellIndex);
        while (currentLevel >= 0)
        {
            const auto& objects = m_cells[cellIndex].GetObjects();
            for (auto* obj : objects)
            {
                if (obj != pObj) collisionList.push_back(obj);
            }

            if (currentLevel == 0) break;  // 繝ｫ繝ｼ繝医そ繝ｫ縺ｫ蛻ｰ驕斐＠縺溘ｉ邨ゆｺ・

            cellIndex = GetParentCellIndex(cellIndex, currentLevel);
            --currentLevel;
        }
        return collisionList;
    }

private:
    // 繧ｻ繝ｫ繧､繝ｳ繝・ャ繧ｯ繧ｹ縺九ｉ繝ｬ繝吶Ν繧帝・ｮ励☆繧・
    uint16_t GetLevelFromIndex(uint16_t cellIndex)
    {
        uint16_t offset = 0;
        for (int level = 0; level <= m_maxLevel; level++)
        {
            uint16_t levelCellCount = 1 << (level * 2);  // 4^level
            uint16_t nextOffset = offset + levelCellCount;

            if (cellIndex < nextOffset)
            {
                return level;
            }
            offset = nextOffset;
        }

        return m_maxLevel;  // 遽・峇螟悶↑繧画怙螟ｧ繝ｬ繝吶Ν
    }

    // 隕ｪ繧ｻ繝ｫ縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ繧貞叙蠕励☆繧・
    uint16_t GetParentCellIndex(uint16_t cellIndex, int currentLevel)
    {
        if (currentLevel == 0) return 0;  // 繝ｫ繝ｼ繝医そ繝ｫ縺ｫ隕ｪ縺ｯ縺ｪ縺・

        // 迴ｾ蝨ｨ縺ｮ繝ｬ繝吶Ν縺ｮ繧ｪ繝輔そ繝・ヨ繧定ｨ育ｮ・
        uint16_t currentOffset = ((1 << (currentLevel * 2)) - 1) / 3;

        // 繝ｬ繝吶Ν蜀・・逶ｸ蟇ｾ菴咲ｽｮ繧貞叙蠕・
        uint16_t relativeIndex = cellIndex - currentOffset;

        // 隕ｪ縺ｮ繝ｬ繝吶Ν蜀・・逶ｸ蟇ｾ菴咲ｽｮ繧定ｨ育ｮ・
        uint16_t parentRelativeIndex = relativeIndex / 4;

        // 隕ｪ縺ｮ繝ｬ繝吶Ν縺ｮ繧ｪ繝輔そ繝・ヨ繧定ｨ育ｮ・
        uint16_t parentOffset = ((1 << ((currentLevel - 1) * 2)) - 1) / 3;

        // 隕ｪ繧ｻ繝ｫ縺ｮ邨ｶ蟇ｾ繧､繝ｳ繝・ャ繧ｯ繧ｹ繧定ｿ斐☆
        return parentOffset + parentRelativeIndex;
    }

    uint16_t Get2DMortonNumber(float worldX, float worldY)
    {
        // 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓吶ｒ4蛻・惠遨ｺ髢薙・逶ｸ蟇ｾ蠎ｧ讓吶↓螟画鋤
        float divX = m_area.z - m_area.x;
        float divY = m_area.w - m_area.y;
        
        float normX = (divX > 0.0001f) ? (worldX - m_area.x) / divX : 0.0f;
        float normY = (divY > 0.0001f) ? (worldY - m_area.y) / divY : 0.0f;

        if (normX < 0) normX = 0;
        if (normY < 0) normY = 0;

        //  譛螟ｧ繝ｬ繝吶Ν縺ｮ隗｣蜒丞ｺｦ縺ｫ螟画鋤
        uint16_t maxCells = 1 << m_maxLevel;

        uint16_t cellX = (uint16_t)(normX * maxCells); 
        uint16_t cellY = (uint16_t)(normY * maxCells); 

        //  遽・峇繝√ぉ繝・け・井ｸ企剞縺ｮ縺ｿ・・
        if (cellX >= maxCells) cellX = maxCells - 1;
        if (cellY >= maxCells) cellY = maxCells - 1;

        // 繝｢繝ｼ繝医Φ逡ｪ蜿ｷ縺ｫ螟画鋤
        return BitSeparate(cellX) | (BitSeparate(cellY) << 1);
    }

    uint16_t GetMortonNumber(const VECTOR2& pos, const VECTOR2& size)
    {
        uint16_t leftTop = Get2DMortonNumber(pos.x, pos.y);
        uint16_t rightBottom = Get2DMortonNumber(pos.x + size.x, pos.y + size.y);

        // 蜷後§繧ｻ繝ｫ縺ｪ繧画怙荳句ｱ､・・axLevel・峨↓螻槭☆繧・
        if (leftTop == rightBottom)
        {
            uint16_t offset = ((1 << (m_maxLevel * 2)) - 1) / 3;
            return offset + leftTop;
        }

        uint16_t xorResult = leftTop ^ rightBottom;
        
        int level = 0;
        if (m_maxLevel > 0)
        {
            uint16_t checkBit = 0x3 << ((m_maxLevel - 1) * 2);
            for (int i = 0; i < m_maxLevel; i++)
            {
                if (xorResult & checkBit)
                {
                    level = i;
                    break;
                }
                checkBit >>= 2;
            }
        }

        uint16_t offset = ((1 << (level * 2)) - 1) / 3;
        uint16_t shift = (m_maxLevel - level) * 2;
        uint16_t cellNumber = leftTop >> shift;
        return offset + cellNumber;
    }

    // 繝薙ャ繝亥・髮｢髢｢謨ｰ・・6繝薙ャ繝亥､縺ｮ蜷・ン繝・ヨ髢薙↓0繧呈諺蜈･縺吶ｋ//
    // 繝｢繝ｼ繝医Φ譖ｲ邱夲ｼ・譖ｲ邱夲ｼ峨・繧､繝ｳ繝・ャ繧ｯ繧ｹ險育ｮ励↓菴ｿ逕ｨ//
    uint16_t BitSeparate(uint16_t n)
    {
        // 8繝薙ャ繝磯俣髫斐〒繝薙ャ繝医ｒ蠎・￡繧・
        n = (n | (n << 8)) & 0x00ff00ff;
        // 4繝薙ャ繝磯俣髫斐〒繝薙ャ繝医ｒ蠎・￡繧・
        n = (n | (n << 4)) & 0x0f0f0f0f;
        // 2繝薙ャ繝磯俣髫斐〒繝薙ャ繝医ｒ蠎・￡繧・
        n = (n | (n << 2)) & 0x33333333;
        // 1繝薙ャ繝磯俣髫斐〒繝薙ャ繝医ｒ蠎・￡繧・
        n = (n | (n << 1)) & 0x55555555;
        return n;
    }

private:
    std::vector<CQuadTreeCell<T>> m_cells;
    int m_maxLevel;
    VECTOR4 m_area;
};
