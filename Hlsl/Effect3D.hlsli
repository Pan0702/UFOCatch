// -----------------------------------------------------------------------
// 
// エフェクトのシェーダー
// 
//                                                              2021.1.11
//                                                         Effect3D.hlsli
// -----------------------------------------------------------------------

// グローバル
Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

// コンスタントバッファ
cbuffer global
{
	matrix g_WVP;       // ワールドから射影までの変換行列
	matrix g_W;         // ワールド変換行列    // -- 2017.8.25
	matrix g_V;         // ビュー変換行列
	matrix g_P;         // 射影変換行列
	float2 g_UVOffset;  // テクスチャ座標のオフセット
	float2 g_UVScale;   // テクスチャ座標の拡縮    // -- 2019.7.16
	float  g_Alpha;     // 透明度
	float  g_Size;      // パーティクルのサイズ  // -- 2018.8.23
	float2 g_Dummy;
};

// 構造体
struct GS_INPUT
{
	float4 Pos : SV_POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

// ----------------------------------------------------------------------------
//
// バーテックスシェーダー ビルボード
//
// ----------------------------------------------------------------------------
PS_INPUT VS_BILL(float4 PosB : POSITION, float2 UV : TEXCOORD)
{
	PS_INPUT Out = (PS_INPUT)0;
	Out.Pos = mul(PosB, g_WVP);
	Out.UV = UV + g_UVOffset;

	return Out;
}

// ----------------------------------------------------------------------------
//
// バーテックスシェーダー ビルボード メッシュ
//
// ----------------------------------------------------------------------------
PS_INPUT VS_BILLMESH(float3 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD, float3 Tangent : TANGENT, float3 Binormal : BINORMAL)
{
	PS_INPUT Out = (PS_INPUT)0;
	float4 PosB = float4(Pos, 1);
	Out.Pos = mul(PosB, g_WVP);
	Out.UV = Tex * g_UVScale + g_UVOffset;

	return Out;
}

// ----------------------------------------------------------------------------
//
// バーテックスシェーダー　パーティクル
//
// ----------------------------------------------------------------------------
GS_INPUT VS_POINT(float4 Pos : POSITION)
{
	GS_INPUT Out = (GS_INPUT)0;
	Out.Pos = Pos;       // ここで頂点を変換しない。なぜならモデル座標のままでジオメトリシェーダーに渡したいから。
	Out.Pos.w = 1.0f;    // 頂点全てのw値を1.0fとする
	return Out;
}
// ----------------------------------------------------------------------------
//
// ジオメトリシェーダー
//
//   ポリゴンの右回りを表面とする      // -- 2024.3.23
//
// ----------------------------------------------------------------------------
[maxvertexcount(4)]
void GS_POINT(point GS_INPUT In[1], inout TriangleStream <PS_INPUT> TriStream)
{

	//  トライアングルストリップで後ろ向きの４角形ポリゴンを作成する。
	//  （視点方向を向かせるとＺの＋方向が視点を向くため）
	//  １辺がg_Sizeの4角形。原点が4角形の中心に来る。  // - 2018.8.23
	PS_INPUT p = (PS_INPUT)0;

	p.Pos = In[0].Pos + float4(g_Size / 2, g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(-g_Size / 2, g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(0, 0);    // -- 2024.3.23
	//p.UV = float2(1, 0);
	TriStream.Append(p);

	p.Pos = In[0].Pos + float4(-g_Size / 2, g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(g_Size / 2, g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(1, 0);      // -- 2024.3.23
	//p.UV = float2(0, 0);
	TriStream.Append(p);

	p.Pos = In[0].Pos + float4(g_Size / 2, -g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(-g_Size / 2, -g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(0, 1);    // -- 2024.3.23
	//p.UV = float2(1, 1);
	TriStream.Append(p);

	p.Pos = In[0].Pos + float4(-g_Size / 2, -g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(g_Size / 2, -g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(1, 1);    // -- 2024.3.23
	//p.UV = float2(0, 1);
	TriStream.Append(p);

	TriStream.RestartStrip();
}
// ----------------------------------------------------------------------------
//
// ピクセルシェーダー
//
// ----------------------------------------------------------------------------
float4 PS(PS_INPUT In) : SV_Target
{
	float4 color;
	color = g_Texture.Sample(g_Sampler, In.UV);
	color.a -= (1 - g_Alpha);
	return saturate(color);
}