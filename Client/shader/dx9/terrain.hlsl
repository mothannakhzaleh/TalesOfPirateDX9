float4x4 mWorldViewProj : WORLDVIEWPROJECTION;
int mLayer;

sampler2D textureSampler0;
sampler2D textureSampler1;

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
	float4 color : COLOR0;
	float2 textureCoordinates0 : TEXCOORD0;
	float2 textureCoordinates1 : TEXCOORD1;
};

VS_OUTPUT main_vs(const VS_INPUT vertex)
{
	VS_OUTPUT output;

	output.position = mul(vertex.position, mWorldViewProj);
	output.color = vertex.color;
	output.textureCoordinates0 = vertex.textureCoordinates0;
	output.textureCoordinates1 = vertex.textureCoordinates1;

	return output;
}

float4 main_ps(const VS_OUTPUT pixel) : COLOR0
{
	float4 color;

	if (mLayer == 0)
	{
		color = tex2D(textureSampler0, pixel.textureCoordinates0) * pixel.color;
	}
	else
	{
		color = tex2D(textureSampler1, pixel.textureCoordinates1) * pixel.color;
		color.a = tex2D(textureSampler0, pixel.textureCoordinates0).a;
	}

	return color;
}
