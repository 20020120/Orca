#include"Obj.hlsli"
#include "SamplerStates.hlsli"

// ----------------------------------- テクスチャ -----------------------------------
Texture2D<float4> gDiffuseTexture : register(t0);

// 使うサンプラーステートの定義
LinerClampSampler

float4 main(ObjVsOut PsIn_) : SV_TARGET
{

	const float3 color = gDiffuseTexture.Sample(SS_LINER_CLAMP, PsIn_.Uv).rgb;
	return float4(color, 1.0f);
}