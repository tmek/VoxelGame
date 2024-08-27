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

#define PI 3.1415926535f

float4 main(PS_INPUT input) : SV_TARGET
{
#define DEBUG_UV 0
#if DEBUG_UV
    // render the UVs for debugging
    return float4(input.TexCoord, 0.0f, input.Color.a);
#else

    
    
    float depth = input.Pos.z / input.Pos.w;

    float b1 = sin(input.Pos.x / 1920.0f * PI);
    float b2 = sin(input.Pos.y / 1080.0f * PI);
    float b = b1*b2;
    
    float vinette_mul = float4(b,b,b,1.0f);
    
    // Calculate linear depth
    // float depth = input.Pos.z / input.Pos.w;
    //
    // float inverseDepth = 1.0f / (depth + 0.001f); // Adding a small value to avoid division by zero
    //
    // // Apply the modulus to create the repeating pattern
    // float unitsPerRepeat = 20;
    // float multiplier = 1 / unitsPerRepeat;
    // float modulatedDepth = 1.0f - fmod(inverseDepth * multiplier, 1.0f);
    //
    // float sd = sin(modulatedDepth);
    // return float4(sd, sd, sd, 1.0f);
    
    // Inverse the depth to compress near values and expand far ones
    // float inverseDepth = 1.0f / (depth + 0.001f); // Adding a small value to avoid division by zero
    //
    // // Apply the modulus to create the repeating pattern
    // float unitsPerRepeat = 20;
    // float multiplier = 1 / unitsPerRepeat;
    // float modulatedDepth = 1.0f - fmod(inverseDepth * multiplier, 1.0f);
    //
    // // Return the modulated depth as grayscale
    // return float4(modulatedDepth, modulatedDepth, modulatedDepth, 1.0f);
    
    // float depth = input.Pos.z / input.Pos.w;
    // float logDepth = log(1.0 + depth) / log(1.2); // Adjust the base to control the scaling
    // return float4(logDepth, logDepth, logDepth, 1.0f);

    //
    // float depth = input.Pos.z / input.Pos.w;
    // float modulatedDepth = fmod(depth * 100.0f, 1.0f); // Repeat every 1/10th of the depth range
    // return float4(modulatedDepth, modulatedDepth, modulatedDepth, 1.0f);

    // // Calculate depth based on the position
    // float depth = input.Pos.z / input.Pos.w;
    // depth = pow(depth, 0.5); // Adjust the exponent to remap depth non-linearly
    // return float4(depth, depth, depth, 1.0f);
    
    // Output depth as grayscale
   // return float4(depth, depth, depth, 1.0f);

    
    
    float4 TexelColor = txDiffuse.Sample(samLinear, input.TexCoord) / 2 + 0.5f;
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
    FogFactor = 0.0f;
    float4 finalColor = lerp(TintedColor, FogColor, FogFactor);
    
    return finalColor ;
#endif
}
