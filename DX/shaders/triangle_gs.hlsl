
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer PositionBuffer : register(cb1)
{
	static float3 gPositions[3] =		// Change to gPositions[3] for a triangle, gPositions[4] for a quad.
	{
		float3( 0.0f, 1.0f, 0.0f ),		// Top left.
		float3( 0.0f, 0.0f, 0.0f ),		// Bottom left.
		//float3( 1.0f, 1.0f, 0.0f ),		// Top right - comment this out for a triangle.
		float3( 1.0f, 0.0f, 0.0f )		// Bottom right.
	};
	float4 padding;		
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Change this to [maxvertexcount(3)] for a triangle, [maxvertexcount(4)] for a quad.
[maxvertexcount(3)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;
	int number_of_vertices = 3;	// How many vertices the shape has, triangle has 3, quad has 4 etc.

	// Change the position vector to be 4 units for proper matrix calculations.
    input[0].position.w = 1.0f;

	// Used for creating the positions for the points.
	for (int i = 0; i < number_of_vertices; i++)
	{
		float3 vPosition = gPositions[i];
		vPosition = mul(vPosition, (float3x3) worldMatrix) + input[0].position;
		
		// Increase the size of the shape by 2.
		/*vPosition.x *= 2;
		vPosition.y *= 2;
		vPosition.z *= 2;*/
		
		output.position = mul(float4(vPosition, 1.0f), viewMatrix);
		output.position = mul(output.position, projectionMatrix);

		output.tex = input[0].tex;
		output.normal = input[0].normal;
		triStream.Append(output);
	}

	triStream.RestartStrip();
}