cbuffer Constants : register(b0)
{
    matrix WorldViewProjection;
    float4 TintColor;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float Depth : TEXCOORD1; // Add depth as a texture coordinate
};

// PS_INPUT main(VS_INPUT input)
// {
//     PS_INPUT output;
//     output.Pos = mul(float4(input.Pos, 1.0f), WorldViewProjection);
//     output.Color = input.Pos.y * .03f + (TintColor * .5f);
//     return output;
// }

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    // Transform the position to clip space
    output.Pos = mul(float4(input.Pos, 1.0f), WorldViewProjection);

    output.Color = input.Color * 1.0f + float4(TintColor.rgb, 0.0f) * .75f;
    output.Normal = input.Normal;
    output.TexCoord = input.TexCoord;
    output.Depth =  output.Pos.z / output.Pos.w;
    
    // Pass the color to the pixel shader
    if(input.Color[3] < 0.0f)
    {
        output.Color = float4(input.Color.rgb, 1.0f);
        output.Depth = -123.0f;
    }
    
    return output;    
    
    // Pass the texture coordinates to the pixel shader
    output.TexCoord = input.TexCoord;

    // Normalize the input normal
    float3 N = normalize(input.Normal);
    output.Normal = N;

    // Define normalized direction to the light source
    float3 L = normalize(float3(0.25f, 1.0f, -.5f));
    
    // Compute the diffuse lighting based on the Lambertian reflection model
    float Ambient = 1.25f;
    float Directional =  max(dot(L, N), 0.0f); 
    float diffuse = 0.0f + Ambient;

    float3 litColor = input.Color * diffuse + TintColor;

    // Calculate and pass the depth
    output.Depth = output.Pos.z / output.Pos.w;
    
    /*
    // calculate fog based on distance, 1 unit = 1 meter
    // make a fog distance
    float fogDistance = 100.0f;
    // make a fog start distance
    float fogStart = 50.0f;
    // calculate the fog amount based on distance
    float fogAmount = saturate((output.Pos.z - fogStart) / fogDistance);
    // make a skycolor literal (based on minecraft sky color)
    float3 skyColor = float3(0.6f, 0.8f, 1.0f);
    // lerp between litColor and skyColor based on fogAmount
    litColor = lerp(litColor, skyColor, fogAmount);
    */
    
    output.Color = float4(litColor, input.Color.a);

    // apply some shadowing base on Y position (lower is darker)
    float HeightShading = (input.Pos.y * .05f) + .5f;
    // clamp to 0-1
    HeightShading = saturate(HeightShading);

    float3 shadowedColor = output.Color.rgb * HeightShading;
    output.Color = float4(shadowedColor, input.Color.a);

    return output;
}
