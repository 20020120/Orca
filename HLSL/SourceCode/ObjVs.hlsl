#include"Obj.hlsli"
#include"Camera.hlsli"
#include"../HLSLMacro/Common.h"


ObjVsOut main( ObjVsIn VsIn_ )
{
    // --------------------------------- �萔�o�b�t�@���擾 --------------------------------
	const ConstantBuffer<Obj> objConstant = GetResourceDH[resourceIndex.ObjectCBufferIndex];
	const ConstantBuffer<Camera> cameraConstant = GetResourceDH[resourceIndex.CameraCBufferIndex];
	ObjVsOut vsOut;
	const float4 localPos = float4(VsIn_.Position, 1.0);
	const float4 worldPos = mul(localPos, objConstant.World);
	const float4 viewPos = mul(worldPos, cameraConstant.ViewMatrix);
	const float4 projPos = mul(viewPos, cameraConstant.ProjMatrix);

	vsOut.Uv = VsIn_.Uv;
	vsOut.Position = projPos;
	vsOut.Color = float4(abs(VsIn_.Normal), 1.0);
	return vsOut;
}