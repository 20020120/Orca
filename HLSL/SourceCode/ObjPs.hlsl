#include"Obj.hlsli"
float4 main(ObjVsOut PsIn_) : SV_TARGET
{
	return float4(PsIn_.Uv, 0.0f, 1.0f);
}