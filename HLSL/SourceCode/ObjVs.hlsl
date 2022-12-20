#include"Obj.hlsli"
ObjVsOut main( ObjVsIn VsIn_ )
{
	ObjVsOut vsOut;
	const float4 localPos = float4(VsIn_.Position, 1.0);
	const float4 worldPos = mul(World, localPos);
	const float4 viewPos = mul(ViewMatrix, worldPos);
	const float4 projPos = mul(ProjMatrix, viewPos);

	vsOut.Position = projPos;
	vsOut.Color = float4(abs(VsIn_.Normal), 1.0);
	return vsOut;
}