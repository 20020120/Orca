#include"Simple.hlsli"

SimpleVsOutput main( SimpleVsInput VsIn_ )
{
	SimpleVsOutput vsOut;
	const float4 localPos = float4(VsIn_.Position, 1.0);
	const float4 worldPos = mul(World, localPos);
	const float4 viewPos = mul(ViewMatrix, worldPos);
	const float4 projPos = mul(ProjMatrix, viewPos);

	vsOut.Position = projPos;
	vsOut.Color = VsIn_.Color;

	return vsOut;
}