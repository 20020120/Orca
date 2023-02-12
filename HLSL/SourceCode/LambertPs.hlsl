#include"Lambert.hlsli"

float4 main(LambertVsOut PsIn_) : SV_TARGET
{
	return PsIn_.Color;
}