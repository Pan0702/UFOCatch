#pragma once

struct StageColl
{
    bool useOBB = true;
    bool useHitGround = false;

    bool operator==(const StageColl& other) const
    {
        return useOBB == other.useOBB && useHitGround == other.useHitGround;
    }

    bool operator!=(const StageColl& other) const
    {
        return !(*this == other);
    }
};
