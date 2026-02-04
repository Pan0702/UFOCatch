#include "Sprite3D.hlsli"

PS_INPUT main(float4 Pos : POSITION, float2 UV : TEXCOORD)
{
    return VS_WORLD(Pos, UV);
}
