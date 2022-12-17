#ifndef Simple_HLSLI
#define Simple_HLSLI

struct SimpleVsInput
{
	float3 Position : POSITION;	// 位置
	float4 Color : COLOT;		// 色
};
struct SimpleVsOutput
{
	float4 Position : SV_POSITION; // 位置
	float4 Color : COLOR; // 色
};
cbuffer Simple : register(b0)
{
	float4x4 World : packoffset(c0);
	float4x4 ViewMatrix : packoffset(c4);
	float4x4 ProjMatrix : packoffset(c8);
}

#endif

