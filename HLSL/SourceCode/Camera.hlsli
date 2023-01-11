// カメラに必要な定数バッファのデータ
cbuffer Camera : register(b0)
{
	float4x4 ViewMatrix;
	float4x4 ProjMatrix;
}