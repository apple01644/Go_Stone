cbuffer cbPerObject : register(b0)
{
	matrix world_transform;
	float4 object_color;
}

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOUR;
};

VS_OUTPUT VS(float3 pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(float4(pos, 1.f), world_transform);
	output.color = object_color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
}