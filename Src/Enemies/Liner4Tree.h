#pragma once
#include <algorithm>
#include <vector>
#include "../Utils/MyMath.h"

#include "QuadTreeCell.h"

template <typename T>
class CLiner4Tree
{
public:
    CLiner4Tree(int level, const VECTOR4& area_)
        : m_maxLevel(level), m_area(area_)
    {
        int size = ((1 << ((level + 1) * 2)) - 1) / 3;
        m_cells.resize(size);
    }

    // 要素を全削除
    void AllClear() { for (auto& cell : m_cells) cell.Clear(); }

    const VECTOR4& GetArea()     const { return m_area; }
    int            GetMaxLevel() const { return m_maxLevel; }

    /// オブジェクトの追加
    /// @param obj オブジェクトのポインタ
    /// @param pos XZ平面での場所
    /// @param size オブジェクトの大きさ
    /// @return オブジェクトを追加できたらtrue
    bool Register(T* obj, const VECTOR2& pos, const VECTOR2& size)
    {
        // オブジェクトが属するセル番号を取得
        uint16_t cellIndex = GetMortonNumber(pos, size);

        if (cellIndex >= m_cells.size()) return false;

        // セルにオブジェクトを追加
        m_cells[cellIndex].Push(obj);

        return true;
    }

    /// 周辺オブジェクトの取得
    /// @param pObj 判定元のオブジェクトポインタ
    /// @param pos XZ平面での場所
    /// @param size オブジェクトの大きさ
    /// @return 近くにいるオブジェクトを返す
    std::vector<T*> GetObjects(T* pObj, const VECTOR2& pos, const VECTOR2& size)
    {
        std::vector<T*> collisionList;
        // オブジェクトのモートン番号から所属セルを取得
        uint16_t cellIndex = GetMortonNumber(pos, size);

        int currentLevel = GetLevelFromIndex(cellIndex);
        while (currentLevel >= 0)
        {
            const auto& objects = m_cells[cellIndex].GetObjects();
            for (auto* obj : objects)
            {
                if (obj != pObj) collisionList.push_back(obj);
            }

            if (currentLevel == 0) break; // ルートセルに到達したら終了

            cellIndex = GetParentCellIndex(cellIndex, currentLevel);
            --currentLevel;
        }
        return collisionList;
    }

private:
    // セルインデックスからレベルを逆算する
    uint16_t GetLevelFromIndex(uint16_t cellIndex)
    {
        uint16_t offset = 0;
        for (int level = 0; level <= m_maxLevel; level++)
        {
            uint16_t levelCellCount = 1 << (level * 2); // 4^level
            uint16_t nextOffset = offset + levelCellCount;

            if (cellIndex < nextOffset)
            {
                return level;
            }
            offset = nextOffset;
        }

        return m_maxLevel; // 範囲外なら最大レベル
    }

    // 親セルのインデックスを取得する
    uint16_t GetParentCellIndex(uint16_t cellIndex, int currentLevel)
    {
        if (currentLevel == 0) return 0; // ルートセルに親はない

        // 現在のレベルのオフセットを計算
        uint16_t currentOffset = ((1 << (currentLevel * 2)) - 1) / 3;

        // レベル内の相対位置を取得
        uint16_t relativeIndex = cellIndex - currentOffset;

        // 親のレベル内の相対位置を計算
        uint16_t parentRelativeIndex = relativeIndex / 4;

        // 親のレベルのオフセットを計算
        uint16_t parentOffset = ((1 << ((currentLevel - 1) * 2)) - 1) / 3;

        // 親セルの絶対インデックスを返す
        return parentOffset + parentRelativeIndex;
    }

    uint16_t Get2DMortonNumber(float worldX, float worldY)
    {
        // ワールド座標を4分木空間の相対座標に変換
        float divX = m_area.z - m_area.x;
        float divY = m_area.w - m_area.y;

        float normX = (divX > 0.0001f) ? (worldX - m_area.x) / divX : 0.0f;
        float normY = (divY > 0.0001f) ? (worldY - m_area.y) / divY : 0.0f;

        normX = std::max<float>(normX, 0);
        normY = std::max<float>(normY, 0);

        // 最大レベルの解像度に変換
        uint16_t maxCells = 1 << m_maxLevel;

        uint16_t cellX = static_cast<uint16_t>(normX * maxCells);
        uint16_t cellY = static_cast<uint16_t>(normY * maxCells);

        // 範囲チェック（上限のみ）
        if (cellX >= maxCells) cellX = maxCells - 1;
        if (cellY >= maxCells) cellY = maxCells - 1;

        // モートン番号に変換
        return BitSeparate(cellX) | (BitSeparate(cellY) << 1);
    }

    uint16_t GetMortonNumber(const VECTOR2& pos, const VECTOR2& size)
    {
        uint16_t leftTop = Get2DMortonNumber(pos.x, pos.y);
        uint16_t rightBottom = Get2DMortonNumber(pos.x + size.x, pos.y + size.y);

        // 同じセルなら最下層（maxLevel）に属する
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

    // ビット分離関数（16ビット値の各ビット間に0を挿入する）
    // モートン曲線（Z曲線）のインデックス計算に使用
    uint16_t BitSeparate(uint16_t n)
    {
        // 8ビット間隔でビットを広げる
        n = (n | (n << 8)) & 0x00ff00ff;
        // 4ビット間隔でビットを広げる
        n = (n | (n << 4)) & 0x0f0f0f0f;
        // 2ビット間隔でビットを広げる
        n = (n | (n << 2)) & 0x33333333;
        // 1ビット間隔でビットを広げる
        n = (n | (n << 1)) & 0x55555555;
        return n;
    }

private:
    std::vector<CQuadTreeCell<T>> m_cells;
    int m_maxLevel;
    VECTOR4 m_area;
};
