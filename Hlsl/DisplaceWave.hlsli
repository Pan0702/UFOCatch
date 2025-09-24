// -----------------------------------------------------------------------
// 
// 波のディスプレースメントマッピングのシェーダー
//                                                              2021.2.4
//  (波に加え、溶岩等の処理に対応する)
//                                                     DisplaceWave.hlsli
// -----------------------------------------------------------------------

// グローバル
Texture2D g_Texture: register(t0);         // テクスチャーは レジスターt(n)     // -- 2021.2.4
Texture2D g_NormalTexture: register(t1);   // 法線マップ
Texture2D g_SpecularTexture: register(t3); // スペキュラーマップ

SamplerState g_samPoint : register(s0);

// グローバル
// ワールドから射影までの変換行列・他(b0)
cbuffer global_0 : register(b0)
{
	matrix g_mW;          // ワールド行列
	matrix g_mWVP;        // ワールドから射影までの変換行列
	float4 g_LightDir;    // ライトの方向ベクトル
	float4 g_EyePos;      // 視点位置
	float4 g_Diffuse;     // ディフューズ色	
	float4 g_DrawInfo;    // 各種情報(使っていない)   // -- 2020.12.15
};
// ディスプレースメントマッピング用の各種データを渡す  // -- 2020.1.24
cbuffer global_1: register(b1)
{
	float3 g_EyePosInv;     // モデルから見た視点位置
	float  g_fMinDistance;  // ポリゴン分割の最小距離
	float  g_fMaxDistance;  // ポリゴン分割の最大小距離
	int    g_iMaxDevide;    // 分割最大数
	float2 g_Height;        // 波の高さ
	float4 g_WaveMove;      // 波の移動量
	float4 g_Specular;      // 鏡面反射
};

struct VS_OUTPUT
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal: NORMAL;
};

struct HS_CONSTANT_OUTPUT
{
	float factor[3]    : SV_TessFactor;
	float inner_factor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float3 Pos  : POSITION;
	float2 Tex  : TEXCOORD0;
};

struct DS_OUTPUT
{
	float4 Pos  : SV_POSITION;
	float2 Tex  : TEXCOORD0;
	float4 PosWorld : TEXCOORD1;        // -- 2019.4.14
};
//
// スペキュラの計算     // -- 2021.2.4
//
float4 CalcSpecular(float4 Color, float4 texSpecular, float4 PosWorld, float3 Normal, float NMbright)
{
	if (texSpecular.w == 1.0f) // スペキュラマップがあるか
	{
		// ハーフベクトルを使ったスペキュラの計算
		float3 ViewDir = -normalize(g_EyePos.xyz - PosWorld.xyz);  // PosWorldからg_EyePosへ向かう視線ベクトル（正規化）  // ベクトルを逆にする   // -- 2021.2.4
		float3 HalfVec = normalize(g_LightDir.xyz + ViewDir);     // ハーフベクトル（視線ベクトルと光源ベククトルの中間ベクトル）（正規化）

		float Gloss = 4;   // 光沢度係数。指数値が大きいほどスペキュラが狭く強くなる。ここでは4としている。
		float Spec = 2.5f * pow(saturate(dot(HalfVec, Normal)), Gloss);   // スペキュラの計算。後でスペキュラマップ値を掛けるため2.5倍にしておく
		Color.rgb = saturate(Color.rgb + Spec * texSpecular.rgb * NMbright);   // スペキュラにスペキュラマップ値を掛けてカラーに加える。

	}
	return Color;
}


// ----------------------------------------------------------------------------
// 
// バーテックスシェーダー
// 
// ----------------------------------------------------------------------------
VS_OUTPUT VS( float3 pos :POSITION ,float2 uv :TEXCOORD0,float3 normal :NORMAL)
{
	VS_OUTPUT Out;

	Out.Pos = pos;

	Out.Tex = uv;
	Out.Tex += g_WaveMove.xy;   // 波のXY方向の変化量をテクスチャ座標に加える
	Out.Normal = normal;        // -- 2021.2.4

	return Out;
}
// ----------------------------------------------------------------------------
//
// ハルシェーダーコンスタント
//
// ----------------------------------------------------------------------------
HS_CONSTANT_OUTPUT HSConstant( InputPatch<VS_OUTPUT, 3> ip, uint pid : SV_PrimitiveID )
{
	HS_CONSTANT_OUTPUT Out;
	float divide = 0;

	float distance = length(g_EyePosInv-ip[0].Pos);  //	  このポリゴン(3頂点の0番目)から視点位置までの距離をはかる

	//float distance = length(g_EyePosInv);	// Worldから視点位置までの距離をはかる

	if(distance<g_fMinDistance) distance = g_fMinDistance;  // 最小距離と最大距離の間になるようにクランプ
	if(distance>g_fMaxDistance) distance = g_fMaxDistance;

	// xは、距離が最大距離と同じ場合は1.0に、最小距離と同じ場合は0.0に、その中間の場合は0.0～1.0になる。
	float x = (distance-g_fMinDistance) / (g_fMaxDistance-g_fMinDistance);
	// 上記の計算で、xは遠近が逆となっているため、1の補数(1-x)をとる。
	divide = (1-x) * g_iMaxDevide+1;

	//divide = 16;      // 分割数の切れ目でポリゴンの境目が気になるときは固定分割とする

	Out.factor[0] = divide;   // 分割数をfactorに設定する
	Out.factor[1] = divide;
	Out.factor[2] = divide;

	Out.inner_factor = divide;

	return Out;
}
// ----------------------------------------------------------------------------
//
// ハルシェーダー
//
// ----------------------------------------------------------------------------
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HSConstant")]
HS_OUTPUT HS( InputPatch<VS_OUTPUT, 3> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID )
{
	HS_OUTPUT Out;
	Out.Pos = ip[cpid].Pos;
	Out.Tex = ip[cpid].Tex;

	return Out;
}
// ----------------------------------------------------------------------------
//
// ドメインシェーダー
//
// ----------------------------------------------------------------------------
[domain("tri")]
DS_OUTPUT DS( HS_CONSTANT_OUTPUT In, float3 DomLoc : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch )
{
	DS_OUTPUT Out;

	// ３つのコントロールポイントのテクスチャ座標に三角形重心位置（x,y,z)を掛けて、実際のテクスチャ座標を求めている。
	// （このとき、0番目のテクスチャ座標の重心位置がDomLoc.xに、1番目がDomLoc.yに、2番目がDomLoc.zに対応する）
	Out.Tex = patch[0].Tex*DomLoc.x + patch[1].Tex*DomLoc.y + patch[2].Tex*DomLoc.z;

	// Heightテクスチャーを使用して、先ほど求めたテクスチャ座標位置の頂点の高さを求める
	// (ノーマルテクスチャをHightテクスチャーとして使用する)
	float4 texHeight = g_NormalTexture.SampleLevel( g_samPoint, Out.Tex, 0 );

	// ３つのコントロールポイントの座標に三角形重心位置（x,y,z)を掛けて、頂点座標を求めている。
	float3 pos = patch[0].Pos*DomLoc.x + patch[1].Pos*DomLoc.y + patch[2].Pos*DomLoc.z;

	// 頂点の高さを加える(y方向に)
	pos.y += (texHeight.x + texHeight.y + texHeight.z) / 3 * g_Height.y;
	float4 pos4 = float4(pos, 1);
	Out.Pos = mul(pos4, g_mWVP);     // ワールド・ビュー・プロジェクション変換をする

	Out.PosWorld = mul(pos4, g_mW);  // ワールド変換をする

	return Out;
}
// ----------------------------------------------------------------------------
//
// ピクセルシェーダ
//
// ----------------------------------------------------------------------------
float4 PS( DS_OUTPUT In ) : SV_Target 
{
	float4 Color;      // 最終出力値
	uint width, height;
	g_Texture.GetDimensions(width, height);  // ディフューズテクスチャのサイズを得る     // -- 2021.2.4

	float4 texSpecular = g_SpecularTexture.Sample(g_samPoint, In.Tex);
	float3 Normal = -normalize(g_NormalTexture.SampleLevel(g_samPoint,In.Tex,0).xyz * 2 - 1);   // ノーマルマップテクスチャから-法線を得る
	float  NMbright = saturate(dot(Normal, g_LightDir.xyz));   // ノーマルマップの光計算

	if (width == 0)  // ディフューズテクスチャがないとき     // -- 2021.2.4
	{
		Color = g_Diffuse;
	}
	else {
		Color = g_Texture.Sample(g_samPoint, In.Tex);
	}

	if (texSpecular.w == 1.0f) // スペキュラマップがあるか     // -- 2021.2.4
	{
		// スペキュラの計算
		Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, Normal, NMbright).rgb;
	}
	else {
		float3 ViewDir = normalize(g_EyePosInv - In.Pos.xyz);
		float3 Reflect = normalize(2 * NMbright * Normal - g_LightDir.xyz);    // 鏡面反射の光計算
		float4 Spec = pow(saturate(dot(Reflect, ViewDir)), 8); // スペキュラの計算

		Color.rgb = Color.rgb * NMbright + g_Specular.rgb * Spec.rgb * 1.3f; // スペキュラを強調
	}

	return saturate(Color);  // 最終出力値を0～１の範囲に制限する

}

