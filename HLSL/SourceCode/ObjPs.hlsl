#include"Obj.hlsli"
float4 main(ObjVsOut PsIn_) : SV_TARGET
{
	return PsIn_.Color;
}