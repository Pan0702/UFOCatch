// -----------------------------------------------------------------------
// 
// シンプルなシェーダー
// 
//                                                              2021.1.11
//                                                           Simple.hlsli
// -----------------------------------------------------------------------

// グローバル変数	
Texture2D g_Texture: register(t0);       // テクスチャーは レジスターt(n)
SamplerState g_samLinear : register(s0);  // サンプラーはレジスターs(n)

// コンスタントバッファ
cbuffer global
{
	matrix g_mW;          // ワールド行列
	matrix g_mWVP;        // ワールドから射影までの変換行列
	float4 g_LightDir;    // ライトの方向ベクトル
	float4 g_EyePos;      // 視点位置
	float4 g_Diffuse;     // ディフューズ色	
	float4 g_DrawInfo;    // 各種情報(使っていない)   // -- 2020.12.15
};

// バーテックスシェーダーの出力構造体
// (ピクセルシェーダーの入力となる)
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD;
};

//
// バーテックスシェーダー
//
VS_OUTPUT VS(float3 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 Pos4 = float4(Pos, 1);
	Out.Pos = mul(Pos4, g_mWVP);           // 頂点をワールド・ビュー・プロジェクション変換する
	Normal = mul(Normal, (float3x3)g_mW);  // 法線をワールド変換する
	Normal = normalize(Normal);            // 法線を正規化する

	if (g_LightDir.x == 0 && g_LightDir.y == 0 && g_LightDir.z == 0)   // 光源のベクトルを指定しないとき
	{
		Out.Color = g_Diffuse;
	}else{
		Out.Color.rgb = g_Diffuse.rgb * (dot(Normal, g_LightDir.xyz) * 0.5f + 0.7f);  //この式はランバートの余弦則
		Out.Color.a   = g_Diffuse.a;   // 透明度はディフューズ色をそのまま使用する
	}
	Out.Tex = Tex;     // テクスチャ座標はそのまま出力

	return Out;
}

//
// ピクセルシェーダー
//
// ・テクスチャの有り無しを判断する方法に以下の2つの方法がある
//   ①　g_DrawInfo.xを見て判断する。1:テクスチャ有り など
//   ②　テクスチャのサイズをGetDimensions()関数で調べ、サイズが０ならテクスチャ無し
//   ここでは、②の方法をとっている
//
float4 PS(VS_OUTPUT In) : SV_Target
{
	uint width, height;
	g_Texture.GetDimensions(width, height);  // テクスチャのサイズを得る
	float4 Color;

	if (width != 0)               // -- 2020.12.15
	{   // テクスチャがあるとき
		float4 texColor = g_Texture.Sample(g_samLinear, In.Tex);  // Sample関数にサンプラーとテクスチャー座標を渡す
		Color.rgb = texColor.rgb * In.Color.rgb;                   // テクスチャ色に光源色を加味する
		Color.a = saturate(texColor.a - (1 - In.Color.a));         // 透明度はディフューズ色を加味する
	}
	else {
		// テクスチャがないとき
		Color = In.Color;
	}
	return Color;
}
