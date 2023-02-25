#include "Camera.hlsli"
#include"Lambert.hlsli"
#include"../HLSLMacro/Common.h"

static float3 lightDirection = { 0.0f, -1.0f, -1.0f };

LambertVsOut main( LambertVsIn VsIn_ ) 
{
	// ÉJÉÅÉâ
	CBuffer<Camera> camera = GetResourceDH[ResourceIndex.mCamera];
	// É{Å[Éì
	CBuffer<Bone> bone = GetResourceDH[ResourceIndex.mNode];

	LambertVsOut vsOut;
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (VsIn_.BornWeight[i] * mul(float4(VsIn_.Position, 1.0), bone.mBoneTransforms[VsIn_.BoneIndices[i]])).xyz;
		n += (VsIn_.BornWeight[i] * mul(float4(VsIn_.Normal.xyz, 0), bone.mBoneTransforms[VsIn_.BoneIndices[i]])).xyz;
	}
	vsOut.Position = mul(float4(p, 1.0f), camera.ViewMatrix);
	vsOut.Position = mul(vsOut.Position, camera.ProjMatrix);

	float3 N = normalize(n);
	float3 L = normalize(lightDirection.xyz);
	float d = dot(L, N);
    const float power = max(0, d) * 0.5f + 0.5f;
	vsOut.Color.rgb = power;
	vsOut.Color.a = 1.0f;
	vsOut.Uv = VsIn_.Uv;

	return vsOut;
}