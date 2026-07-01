#pragma once

/// <summary>ステージで使う Stage Coll の情報と処理をまとめる型</summary>
struct StageColl
{
    bool useOBB = true;
    bool useHitGround = false;

    /// operator== の処理を行う
    /// @param other other に渡す値
    /// @return 成功または条件を満たす場合 true
    bool operator==(const StageColl& other) const
    {
        return useOBB == other.useOBB && useHitGround == other.useHitGround;
    }

    /// operator!= の処理を行う
    /// @param other other に渡す値
    /// @return 成功または条件を満たす場合 true
    bool operator!=(const StageColl& other) const
    {
        return !(*this == other);
    }
};
