#include"Simple.hlsli"
float4 main(SimpleVsOutput PsIn_) : SV_TARGET
{
	return PsIn_.Color;
}