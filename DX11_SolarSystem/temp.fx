//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);
TextureCube cubemap : register(t1);

cbuffer MatrixBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

cbuffer LightBuffer
{
	float3 lightDir;
	float3 lightPos;
	float lightRange;
	float3 lightAtt;
	float4 lightAmbient;
	float4 lightDiffuse;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
};

struct VS_OUTPUT_SKY_MAP
{
	float4 Pos : SV_POSITION;
	float3 TexCoord : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = mul( input.Pos, World );
    output.worldPos = mul(input.Pos, World);

    output.Tex = input.Tex;

    output.Norm = normalize(input.Norm);

    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Norm = mul( input.Norm, (float3x3)World );

    return output;
};

VS_OUTPUT_SKY_MAP SKYMAP_VS(PS_INPUT input)
{
	VS_OUTPUT_SKY_MAP output = (VS_OUTPUT_SKY_MAP)0;

	output.Pos = mul( input.Pos, World );
	output.Pos = mul( output.Pos, View );
   	output.Pos = mul( output.Pos, Projection );

	output.TexCoord = input.Pos;

	return output;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	input.Norm = normalize(input.Norm);
   	float4 diffuse = shaderTexture.Sample(sampleType, input.Tex);

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightToPixelVec = lightPos - input.worldPos;

	float d = length(lightToPixelVec);

	float3 finalAmbient = diffuse * lightAmbient;

	if (d > lightRange)
		return float4(finalAmbient, diffuse.a);

	lightToPixelVec /= d;

	float howMuchLight = dot(lightToPixelVec, input.Norm);

	if (howMuchLight > 0.0f)
	{
		finalColor += howMuchLight * diffuse * lightDiffuse;
		finalColor /= lightAtt[0] + (lightAtt[1] * d) + (lightAtt[2] * (d * d));
	}

	finalColor = saturate(finalColor + finalAmbient);

	return float4(finalColor, diffuse.a);
};

float4 PSSolid( PS_INPUT input) : SV_Target
{
    return shaderTexture.Sample(sampleType, input.Tex);
};

float4 SKYMAP_PS(VS_OUTPUT_SKY_MAP input) : SV_TARGET
{
    return cubemap.Sample(sampleType, input.TexCoord);
};