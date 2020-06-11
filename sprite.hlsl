cbuffer cbPerObject : register(b0)
{
	matrix world_transform;
	float4 object_color;
}

Texture2D sprite : register(t0);
sampler samLinear =
sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 coord : TEX_COORD;
	float4 color : COLOUR;
};

VS_OUTPUT VS(float3 pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(float4(pos, 1.f), world_transform);
	output.coord = pos.xy;
	output.color = object_color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{ 
	return sprite.Sample(samLinear, input.coord) + input.color;
}