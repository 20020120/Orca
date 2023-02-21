#ifndef Obj_HLSLI
#define Obj_HLSLI

struct ResourceIndex
{
	uint CameraCBufferIndex;
	uint ObjectCBufferIndex;
};
ConstantBuffer<ResourceIndex> resourceIndex : register(b0);

struct ObjVsIn
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Uv : UV;
};
struct ObjVsOut
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float4 Color : COLOR;
};

// ----------------------------------- 定数バッファ ----------------------------------
struct Obj
{
	float4x4 World;
};
#endif

