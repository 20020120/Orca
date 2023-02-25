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

static const uint MaxBones = 128;
struct Bone
{
	matrix mBoneTransforms[MaxBones];
};


// ----------------------------------- 定数バッファ ----------------------------------
struct ResourceIndex
{
	uint mCamera;
	uint mNode;
};
ConstantBuffer<ResourceIndex> ResourceIndex : register(b0);

#endif

