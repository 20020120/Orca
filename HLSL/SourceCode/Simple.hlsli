#ifndef Simple_HLSLI
#define Simple_HLSLI

struct SimpleVsInput
{
	float3 Position : POSITION;	// �ʒu
	float4 Color : COLOT;		// �F
};
struct SimpleVsOutput
{
	float4 Position : SV_POSITION; // �ʒu
	float4 Color : COLOR; // �F
};
cbuffer Simple : register(b0)
{
	float4x4 World : packoffset(c0);
	float4x4 ViewMatrix : packoffset(c4);
	float4x4 ProjMatrix : packoffset(c8);
}

#endif

