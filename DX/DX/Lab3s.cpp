// Lab3.cpp
// Lab 2 example, simple coloured triangle mesh
#include "Lab3s.h"

Lab3s::Lab3s(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	rotation = 0.0f;

	// Creating a sphere mesh.
	m_SphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");

	// Setting up the lighting.
	m_Light = new Light();
	m_Light->SetAmbientColour(0.4f, 0.4f, 0.4f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 0.0f);
	m_Light->SetSpecularPower(2.0f);
	m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// Setting up the different shaders.
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_SpecularLightShader = new SpecularLightShader(m_Direct3D->GetDevice(), hwnd);
	m_LightShader = new LightShader(m_Direct3D->GetDevice(), hwnd);
}

Lab3s::~Lab3s()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_SphereMesh)
	{
		delete m_SphereMesh;
		m_SphereMesh = 0;
	}

	if (m_ColourShader)
	{
		delete m_ColourShader;
		m_ColourShader = 0;
	}

	if (m_TextureShader)
	{
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_SpecularLightShader)
	{
		delete m_SpecularLightShader;
		m_SpecularLightShader = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
}

bool Lab3s::Frame()
{
	bool result;

	rotation += 0.01f;

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

bool Lab3s::Render()
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

	// Rotating the sphere.
	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, rotation, 0.0f);

	// First set of geometry data.
	// Send geometry data (from mesh)
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());

	// Set shader parameters (matrices and texture)
	//m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(), m_Light);
	m_SpecularLightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(), m_Light, m_Camera);
	//m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture());
	
	// Render object (combination of mesh geometry and shader process
	//m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	m_SpecularLightShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	//m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


