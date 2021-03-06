// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColor, finalColour, invertedColour;
	float4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
	
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = texture0.Sample(Sampler0, input.tex);
	
	// Blending red with texture colours.
	//invertedColour = (textureColor.a - textureColor.rgb, textureColour.a, 0.0f, 1.0f);
	//finalColour = lerp(textureColor, red, 0.5f);
	
	return textureColor;
	//return finalColour;
}