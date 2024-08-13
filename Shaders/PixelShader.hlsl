Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float Depth : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
#define DEBUG_UV 1
#if DEBUG_UV
    // render the UVs for debugging
    return float4(input.TexCoord, 0.0f, input.Color.a);
#else
    
    float4 TexelColor = txDiffuse.Sample(samLinear, input.TexCoord) / 2.0f + 0.5f;
    float4 TintedColor = input.Color * TexelColor;
    
    // calculate fog based on distance, 1 unit = 1 meter
    float4 FogColor = float4(0.4706f, 0.6549f, 1.0f, 1.0f);
    
    // Define the fog parameters
    float FogEnd = 32 * 16; // assuming 32 chunks wide
    float FogStart = FogEnd * 0.25f;
    
    // Calculate the fog factor
    float FogDistance = input.Depth * input.Pos.w;
    float FogFactor = 1.0f - saturate((FogEnd - FogDistance) / (FogEnd - FogStart));

    // Interpolate between the vertex color and the fog color
    float4 finalColor = lerp(TintedColor, FogColor, FogFactor);
    
    return finalColor;
#endif
}
