#include"Obj.hlsli"

// ----------------------------------- テクスチャ -----------------------------------
Texture2D<float4> gDiffuseTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 main(ObjVsOut PsIn_) : SV_TARGET
{
	const float4 color = gDiffuseTexture.Sample(gSamplerState, PsIn_.Uv);
	return color;
}