//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
SamplerState sampleType : register(s1);
TextureCube cubemap : register(t1);

cbuffer MatrixBuffer : register(b1)
{	
	matrix WVP;
	matrix World;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT2
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Norm : NORMAL;
};

struct VS_OUTPUT_SKY_MAP
{
	float4 Pos : SV_POSITION;
	float3 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT_SKY_MAP SKYMAP_VS(VS_INPUT2 input)
{
	VS_OUTPUT_SKY_MAP output = (VS_OUTPUT_SKY_MAP)0;

	output.Pos = mul(input.Pos, WVP);
	output.Tex = input.Pos;

	return output;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 SKYMAP_PS(VS_OUTPUT_SKY_MAP input) : SV_Target
{
	return cubemap.Sample(sampleType, input.Tex);
};
