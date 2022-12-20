#ifndef Obj_HLSLI
#define Obj_HLSLI

struct ObjVsIn
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};
struct ObjVsOut
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};
cbuffer Obj : register(b0)
{
	float4x4 World : packoffset(c0);
	float4x4 ViewMatrix : packoffset(c4);
	float4x4 ProjMatrix : packoffset(c8);
}

#endif

