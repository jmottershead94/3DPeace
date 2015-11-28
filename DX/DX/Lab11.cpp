// Lab11.cpp
#include "Lab11.h"

Lab11::Lab11(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Setting up local variables.
	m_Input = in;

	// Shaders.
	m_GeometryShader = new GeometryShader(m_Direct3D->GetDevice(), hwnd);
	
	// Meshes.
	// Fill the points vector with point meshes.
	for (int i = 0; i < 1; i++)
	{
		// Pushing three point meshes into the points vector.
		m_Points.push_back(GetPointMesh());
	}
}

Lab11::~Lab11()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_GeometryShader)
	{
		delete m_GeometryShader;
		m_GeometryShader = nullptr;
	}

	// If there are point meshes in the points vector.
	if (!m_Points.empty())
	{
		// Empty the points.
		m_Points.clear();
	}
}

void Lab11::Controls(float dt)
{
	
}

bool Lab11::Frame()
{
	bool result;

	result = BaseApplication::Frame();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Lab11::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();
	
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Handle user input.
	Controls(m_Timer->GetTime());
	
	// Looping through the points vector.
	for (auto& points : m_Points)
	{
		points->SendData(m_Direct3D->GetDeviceContext());
		
		m_GeometryShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, points->GetTexture());
		m_GeometryShader->Render(m_Direct3D->GetDeviceContext(), points->GetIndexCount());

		//worldMatrix = XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	}

	// If it is a triangle point mesh, place this in to make it the right way around.
	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}