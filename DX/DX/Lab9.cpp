// Lab9.cpp
#include "Lab9.h"

Lab9::Lab9(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Setting up local variables.
	m_TessellationFactor = 8.0f;
	m_Input = in;
	
	// Creating new mesh.
	m_TessellationMesh = new TessellationMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
	m_NewTessellationMesh = new Lab9TessellationMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");

	// Setting up the different shaders.
	m_TessellationShader = new TessellationShader(m_Direct3D->GetDevice(), hwnd);
	m_QuadTessellationShader = new QuadTessellationShader(m_Direct3D->GetDevice(), hwnd);

	// Start off with wireframe turned on.
	m_Direct3D->TurnOnWireframe();
}

Lab9::~Lab9()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_TessellationMesh)
	{
		delete m_TessellationMesh;
		m_TessellationMesh = 0;
	}

	if (m_NewTessellationMesh)
	{
		delete m_NewTessellationMesh;
		m_NewTessellationMesh = 0;
	}

	if (m_TessellationShader)
	{
		delete m_TessellationShader;
		m_TessellationShader = 0;
	}

	if (m_QuadTessellationShader)
	{
		delete m_QuadTessellationShader;
		m_QuadTessellationShader = 0;
	}
}

void Lab9::Controls(float dt)
{
	if (m_Input->isKeyDown(VK_ADD))
	{
		m_TessellationFactor++;
	}
	else if (m_Input->isKeyDown(VK_SUBTRACT) && m_TessellationFactor > 2.0f)
	{
		m_TessellationFactor--;
	}
}

bool Lab9::Frame()
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

bool Lab9::Render()
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

	// Geometry Data.
	m_TessellationMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shaders.
	m_TessellationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_TessellationMesh->GetTexture(), m_TessellationFactor);
	m_TessellationShader->Render(m_Direct3D->GetDeviceContext(), m_TessellationMesh->GetIndexCount());

	// Moving along.
	worldMatrix = XMMatrixTranslation(2.0f, 0.0f, 0.0f);

	// Second set of geometry.
	m_NewTessellationMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shaders.
	m_QuadTessellationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_NewTessellationMesh->GetTexture(), m_TessellationFactor);
	m_QuadTessellationShader->Render(m_Direct3D->GetDeviceContext(), m_NewTessellationMesh->GetIndexCount());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
