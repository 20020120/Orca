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

cbuffer Obj : register(b1)
{
	float4x4 World;
}
#endif

