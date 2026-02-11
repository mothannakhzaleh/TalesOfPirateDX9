float4x4 mWorldViewProj : WORLDVIEWPROJECTION;

struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR0;
	float2 textureCoordinates0 : TEXCOORD0;
	float2 textureCoordinates1 : TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 position : POSITION;
	float  depth : TEXCOORD0;
};

VS_OUTPUT main_vs(const VS_INPUT vertex)
{
	VS_OUTPUT output;

	output.position = mul(vertex.position, mWorldViewProj);
	output.depth = output.position.z * 0.01f;

	return output;
}

float4 main_ps(const VS_OUTPUT pixel) : COLOR0
{
	// return float4(1, 0, 1, 1);
	return float4(pixel.depth, pixel.depth, pixel.depth, 1);
}
