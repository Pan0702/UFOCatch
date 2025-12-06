// -----------------------------------------------------------------------
// 
// ３Ｄスプライトのシェーダー
// 
//                                                              2021.1.11
//                                                          Sprite3D.hlsli
// -----------------------------------------------------------------------

// グローバル
Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

// コンスタントバッファ
cbuffer global_0:register(b0)
{
    matrix g_WVP; // ワールドから射影までの変換行列  // -- 2018.8.10
    matrix g_W; // ワールド変換行列のみ
    float g_ViewPortWidth; // ビューポート（スクリーン）横サイズ
    float g_ViewPortHeight; // ビューポート（スクリーン）縦サイズ
    float2 g_UVOffset; // テクスチャ座標　オフセット
    float4 g_Diffuse; // ディフューズカラー                        // 2020.1.24
    float4 g_MatInfo; // マテリアル関連情報　x:テクスチャ有り無し  // 2017.10.8
};

// 構造体
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

//
// バーテックスシェーダー
//
PS_INPUT VS(float4 Pos : POSITION, float2 UV : TEXCOORD)
{
    PS_INPUT Out;

    Out.Pos = mul(Pos, g_W);

    Out.Pos.x = (Out.Pos.x / g_ViewPortWidth) * 2 - 1;
    Out.Pos.y = 1 - (Out.Pos.y / g_ViewPortHeight) * 2;

    Out.UV = UV + g_UVOffset;

    return Out;
}

//
// バーテックスシェーダー 3Dスプライト（ビルボード）
//
PS_INPUT VS_BILL(float4 PosB : POSITION, float2 UV : TEXCOORD)
{
    PS_INPUT Out = (PS_INPUT)0;
    Out.Pos = mul(PosB, g_WVP);
    Out.UV = UV + g_UVOffset;

    return Out;
}

//
// ピクセルシェーダー
//
// ・テクスチャの有り無しを判断する方法に以下の2つの方法がある
//   ?@　g_MatInfo.xを見て判断する。1:テクスチャ有り
//   ?A　テクスチャのサイズをGetDimensions()関数で調べ、サイズが０ならテクスチャ無し
//   ここでは、?@の方法をとっている
//
float4 PS(PS_INPUT In) : SV_Target
{
    //uint width, height;
    //g_Texture.GetDimensions(width, height);  // テクスチャのサイズを得る  // -- 2020.1.15

    float4 Color;
    if (g_MatInfo.x == 1)
    //if (width != 0)
    {
        // テクスチャ有りのとき
        float4 texColor = g_Texture.Sample(g_Sampler, In.UV);
        Color.rgb = texColor.rgb * g_Diffuse.rgb;
        Color.a = texColor.a - (1 - g_Diffuse.a); // -- 2020.1.15
        //円の角度制限が有りの場合
        if (g_MatInfo.w == 1)
        {
            float PI = 3.14159265f;

            // UV座標から中心を引いて相対位置を計算
            // UVオフセットを引いて正規化（0～1の範囲に）
            float2 normalizedUV = In.UV - g_UVOffset;
            float2 pos = normalizedUV - float2(0.5, 0.5);

            // 12時方向を0度、時計回りに角度を計算
            // UV座標系: Y軸は下が0、上が1
            float angle = atan2(pos.x, -pos.y);

            // 0～2πの範囲に正規化
            if (angle < 0.0f) {
                angle += 2.0f * PI;
            }

            // 開始角度から終了角度の範囲外を破棄
            if (angle < g_MatInfo.y || angle > g_MatInfo.z) {
                discard;
            }
        }
    }
    else
    {
        // テクスチャなしのとき
        Color = g_Diffuse;
    }
    return saturate(Color);
}
