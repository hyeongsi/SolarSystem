//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightPosition;
	float padding;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Norm : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Norm : NORMAL;
	float3 diffuse : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = mul( input.Pos, World );
   
    output.Tex = input.Tex;

    output.Norm = normalize(output.Norm);

    float3 lightDir = normalize((float3)output.Pos - lightPosition);

    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Norm = mul( input.Norm, (float3x3)World );

    float3 worldNormal = mul(input.Norm, (float3x3)World);
    worldNormal = normalize(worldNormal);
    output.diffuse = dot(-lightDir, worldNormal);

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 textureColor = shaderTexture.Sample(sampleType, input.Tex);
   
    float4 diffuse = saturate(float4(input.diffuse, 1));

    float4 outputColor = saturate(diffuse * diffuseColor);

    outputColor = outputColor * textureColor;

    return outputColor;
}
