// Lab2.cpp
// Lab 2 example, simple coloured triangle mesh
#include "lab2.h"

Lab2::Lab2(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	rotation = 0.0f;

	// Creating a quad mesh.
	m_QuadMesh = new QuadMesh(m_Direct3D->GetDevice(), L"../res/bunny.png");
	m_SecondQuadMesh = new QuadMesh(m_Direct3D->GetDevice(), L"../res/WINDOW.png");

	m_ColourShader = new ColourShader(m_Direct3D->GetDevice(), hwnd);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
}

Lab2::~Lab2()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_QuadMesh)
	{
		delete m_QuadMesh;
		m_QuadMesh = 0;
	}

	if (m_ColourShader)
	{
		delete m_ColourShader;
		m_ColourShader = 0;
	}
}

bool Lab2::Frame()
{
	bool result;

	rotation += 0.1f;

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

bool Lab2::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// First set of geometry data.
	// Send geometry data (from mesh)
	m_QuadMesh->SendData(m_Direct3D->GetDeviceContext());
	
	// Set shader parameters (matrices and texture)
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_QuadMesh->GetTexture());
	
	// Render object (combination of mesh geometry and shader process
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_QuadMesh->GetIndexCount());
	
	// Translation by 2 units to the right.
	worldMatrix = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, rotation);
	//worldMatrix = XMMatrixScaling(2.0f, 1.0f, 1.0f);

	// Second set of geometry data.
	// Setting up the second quad.
	m_SecondQuadMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SecondQuadMesh->GetTexture());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SecondQuadMesh->GetIndexCount());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


