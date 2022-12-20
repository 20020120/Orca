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
	row_major float4x4 World : packoffset(c0);
	row_major float4x4 ViewMatrix : packoffset(c4);
	row_major float4x4 ProjMatrix : packoffset(c8);
}

#endif

