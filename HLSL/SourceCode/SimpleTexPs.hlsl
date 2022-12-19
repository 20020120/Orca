#include"SimpleTex.hlsli"

SamplerState ColorSmp : register(s0);
Texture2D ColorMap : register(t0);

float4 main(const SimpleTexVsOutput PsIn_) : SV_TARGET
{
	float4 outPut = ColorMap.Sample(ColorSmp, PsIn_.TexCoord);
	return outPut;
}