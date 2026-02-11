float4x4 mWorldViewProj : WORLDVIEWPROJECTION;

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR0;
	float2 textureCoordinates : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : POSITION;
	float  depth : TEXCOORD0;
};

VS_OUTPUT main_vs(const VS_INPUT vertex)
{
	VS_OUTPUT output;

	output.position = mul(float4(vertex.position, 1), mWorldViewProj);
	output.depth = output.position.z * 0.01f;

	return output;
}
