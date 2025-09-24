// -----------------------------------------------------------------------
// 
// Ｆｂｘスタティックメッシュのシェーダー
// 
//                                                              2021.6.11
//                                                    FbxStaticMesh.hlsli
// -----------------------------------------------------------------------

// グローバル変数	
Texture2D g_Texture: register(t0);   //テクスチャーは レジスターt(n)
Texture2D g_NormalTexture : register(t1);   // 法線マップ
Texture2D g_SpecularTexture : register(t3);   // スペキュラーマップ

SamplerState g_samLinear : register(s0);//サンプラーはレジスターs(n)


//  (コンスタントバッファ)
cbuffer global : register(b0)               // -- 2020.12.15
{
	matrix g_mW;          // ワールド行列
	matrix g_mWVP;        // ワールドから射影までの変換行列
	float4 g_LightDir;    // ライトの方向ベクトル
	float4 g_EyePos;      // 視点位置
	float4 g_Diffuse;     // ディフューズ色	
	float4 g_DrawInfo;    // 各種情報(使っていない)   // -- 2020.12.15
};

// マテリアルカラーのコンスタントバッファ
cbuffer global : register(b3)               // -- 2020.12.15
{
	float4 g_MatDiffuse = float4(0.5, 0.5, 0.5, 1);   // ディフューズ色	
	float4 g_MatSpecular = float4(0, 0, 0, 0);        // スペキュラ色
};

// バーテックスシェーダーの出力構造体
// (ピクセルシェーダーの入力となる)
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float4 lightTangentSpace : TEXCOORD1; // 接空間に変換されたライトベクトル

	float4 PosWorld:TEXCOORD2;
};

// 接空間行列の逆行列を算出
float4x4 InverseTangentMatrix(float3 tangent, float3 binormal, float3 normal)
{
	tangent = normalize(mul(tangent, (float3x3)g_mW));     // 接線をワールド変換する    // -- 2021.6.11
	binormal = normalize(mul(binormal, (float3x3)g_mW));   // 従法線をワールド変換する
	normal = normalize(mul(normal, (float3x3)g_mW));       // 法線をワールド変換する

	float4x4 mat = { float4(tangent, 0.0f),
		float4(binormal, 0.0f),
		float4(normal, 0.0f),
		{ 0.0f, 0.0f, 0.0f, 1.0f } };

	return transpose(mat); // 転置行列にする
}

//
// スペキュラの計算
//
float4 CalcSpecular(float4 Color, float4 texSpecular, float4 PosWorld, float3 Normal, float NMbright)
{
	if (texSpecular.w == 1.0f) // スペキュラマップがあるか
	{
		// ハーフベクトルを使ったスペキュラの計算
		float3 ViewDir = normalize( g_EyePos.xyz - PosWorld.xyz);  // PosWorldからg_EyePosへ向かう視線ベクトル（正規化）
		float3 HalfVec = normalize( g_LightDir.xyz + ViewDir);     // ハーフベクトル（視線ベクトルと光源ベククトルの中間ベクトル）（正規化）

		float Gloss = 4;   // 光沢度係数。指数値が大きいほどスペキュラが狭く強くなる。ここでは4としている。
		float Spec = 2.5f * pow( saturate(dot(HalfVec, Normal)), Gloss );   // スペキュラの計算。後でスペキュラマップ値を掛けるため2.5倍にしておく
		Color.rgb = saturate(Color.rgb + Spec * texSpecular.rgb * NMbright);   // スペキュラにスペキュラマップ値を掛けてカラーに加える。

	}
	return Color;
}

//
// バーテックスシェーダー
//
VS_OUTPUT VS(float3 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD, float3 Tangent : TANGENT, float3 Binormal : BINORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 Pos4 = float4(Pos, 1);
	float4x4 inverseTangentMat;

	Out.Pos = mul(Pos4, g_mWVP);  // 頂点をワールド・ビュー・プロジェクション変換する

	// 接線の計算
	// 接空間行列の逆行列を算出
	inverseTangentMat = InverseTangentMatrix(normalize(Tangent), normalize(Binormal), normalize(Normal));

	// ライトベクトルを接空間上に変換
	Out.lightTangentSpace = mul(float4(g_LightDir.xyz, 1.0), inverseTangentMat);

	// 法線をワールド変換する
	Out.Normal = normalize(mul(Normal, (float3x3)g_mW));

	// テクスチャ座標はそのまま出力
	Out.Tex = Tex;

	Out.PosWorld = mul(Pos4, g_mW);  // 頂点をワールド変換する

	return Out;
}

//
// ピクセルシェーダー
//
float4 PS(VS_OUTPUT In) : SV_Target
{
	float4 Color;      // 最終出力値
	uint width, height;
	g_Texture.GetDimensions(width, height);  // ディフューズテクスチャのサイズを得る  // -- 2020.12.15

	// テクスチャカラー
	float4 texColor;
	float4 texSpecular;

	if (width == 0)  // ディフューズテクスチャがないとき  // -- 2020.12.15
	{
		texColor = g_MatDiffuse;      // -- 2020.12.15
		texSpecular = g_MatSpecular;      // -- 2020.12.15
	}
	else {
	    texColor = g_Texture.Sample(g_samLinear, In.Tex);
	    texSpecular = g_SpecularTexture.Sample(g_samLinear, In.Tex);
	}
	float4 texNormal = g_NormalTexture.Sample(g_samLinear, In.Tex);

	// 光源の方向が設定されているかどうかのチェック
	if (g_LightDir.x == 0 && g_LightDir.y == 0 && g_LightDir.z == 0)
	{
		// 光の方向が0,0,0の場合は光の計算をしないで描画
		Color.rgb = texColor.rgb * g_Diffuse.rgb;
		Color.a = texColor.a - (1 - g_Diffuse.a); 
	}
	else {
		// 光の方向が設定されている場合
		if (texNormal.w == 1.0f) {   // 法線マップがあるかどうかチェック

			// 法線マップがある場合は法線マッピング付きのライティング
			// 法線マッピングの明るさ計算
			float4 normalVector = normalize( 2.0 * texNormal - 1.0f);  // 法線マップをベクトルへ変換(法線マップの色は0～1.0。これを-1.0～1.0のベクトル範囲にする)
			float NMbright = max(0, dot( normalVector, normalize(In.lightTangentSpace)));// ライトの向きと法線マップの法線とで明度算出。マイナスは0に補正。

			// 光源の方向計算
			float halflamb = max(0, dot(normalize(In.Normal), g_LightDir.xyz));
			halflamb = halflamb * 0.6f + 0.7f;   // ちょっと明るめに

			// 法線と光源方向を加味した最終的な色の計算
			Color.rgb = texColor.rgb * NMbright * halflamb * g_Diffuse.rgb;

			// スペキュラの計算
			Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, In.Normal, NMbright).rgb;
			// 透明色の計算
			Color.a = texColor.a - (1 - g_Diffuse.a); 
		}
		else {

			// 法線マップがセットされていない場合は、ハーフランバートで描画
			float halflamb = max(0, dot(normalize(In.Normal), g_LightDir.xyz));
			//halflamb = halflamb * 0.4f + 0.7f;
			halflamb = halflamb * 0.7f + 0.6f;      // -- 2021.6.11

			// 光源方向を加味した最終的な色の計算
			Color.rgb = texColor.rgb * halflamb * g_Diffuse.rgb;

			// スペキュラの計算
			Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, In.Normal, 1).rgb;
			// 透明色の計算
			Color.a = texColor.a - (1 - g_Diffuse.a);
		}
	}
	return saturate(Color);  // 最終出力値を0～１の範囲に制限する
}
