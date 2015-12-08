// Jason Mottershead, 1300455.

// Particle vertex shader file.
// This will output a colour based on the lighting calculations.

// Vertex input.
struct InputType
{
    float4 position : POSITION;
	float4 colour : COLOR;
};

// Vertex output / geometry input.
struct OutputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

InputType main(InputType input)
{
	OutputType output;

	// The last element in the position float, 1.0f for matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = input.position;

	// The colour of the current vertex.
	output.colour = input.colour;

	return output;
}