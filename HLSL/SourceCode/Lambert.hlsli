// LambertShader用のHlslヘッダファイル

#ifndef Lambert_HLSLI
#define Lambert_HLSLI

struct LambertVsIn
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 Uv : UV;
	float4 Color : COLOR;
	float4 BornWeight : WEIGHT;
	uint4 BoneIndices : BONES;
};

struct LambertVsOut
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float4 Color : COLOR;
};

// ----------------------------------- 定数バッファ ----------------------------------
cbuffer Lambert : register(b1)
{
	float4x4 World;
}


#endif

