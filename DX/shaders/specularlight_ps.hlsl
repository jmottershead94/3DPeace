// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColour;
    float4 diffuseColour;
    float3 direction;
	float specularPower;
	float4 specularColour;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float lightIntensity;
    float4 colour;
	float3 reflection;
	float specular;
	float4 finalSpecular = {0.0f, 0.0f, 0.0f, 1.0f};

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = shaderTexture.Sample(SampleType, input.tex);

	// Setting a default amount of colour.
	colour = ambientColour;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, -direction));

	// If the light intensity is greater than 0.
	if (lightIntensity > 0.0f)
	{
		// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
		// Same calculation as before.
		colour += (diffuseColour * lightIntensity);
		colour = saturate(colour);

		// Calculating the reflection matrix based on the light intensity, normal vector and light direction.
		reflection = reflect(direction, input.normal);

		// Determine the amount of specular light based on the reflection vector, viewing direction and specular power.
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		// Sum up the specular light.
		finalSpecular = (specularColour * specular);
	}

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    colour = colour * textureColour;

	// Add the specular component last to the output colour.
	colour += finalSpecular;
	colour = saturate(colour);

	return colour;
}

