#ifndef SimpleTex_H
#define SimpleTex_H

struct SimpleTexVsInput
{
	float3 Position : POSITION; // ˆÊ’u
	float2 TexCoord : TEXCOORD; // UV
};
struct SimpleTexVsOutput
{
	float4 Position : SV_POSITION; // ˆÊ’u
	float2 TexCoord : TEXCOORD; // UV
};
cbuffer Simple : register(b0)
{
	float4x4 World : packoffset(c0);
	float4x4 ViewMatrix : packoffset(c4);
	float4x4 ProjMatrix : packoffset(c8);
}

#endif
