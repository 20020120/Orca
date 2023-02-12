#include "Camera.hlsli"
#include"Lambert.hlsli"

static float3 lightDirection = { 0.0f, -1.0f, -1.0f };

LambertVsOut main( LambertVsIn VsIn_ ) 
{
	LambertVsOut vsOut;
#if 0
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]])).xyz;
		n += (boneWeights[i] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
	}
#else
	float3 p = VsIn_.Position.xyz;
	float3 n = VsIn_.Normal;
#endif
	vsOut.Position = mul(float4(p, 1.0f), World);
	vsOut.Position = mul(vsOut.Position, ViewMatrix);
	vsOut.Position = mul(vsOut.Position, ProjMatrix);

	float3 N = normalize(n);
	float3 L = normalize(lightDirection.xyz);
	float d = dot(L, N);
    const float power = max(0, d) * 0.5f + 0.5f;
	vsOut.Color.rgb = power;
	vsOut.Color.a = 1.0f;
	vsOut.Uv = VsIn_.Uv;

	return vsOut;
}