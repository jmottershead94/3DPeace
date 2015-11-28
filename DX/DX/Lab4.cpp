// Lab4.cpp
#include "Lab4.h"

Lab4::Lab4(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Creating a sphere mesh.
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/White.png");

	// Setting up the lighting.
	// Setting up a red light on the left.
	m_MultipleLights[0] = new Light();
	m_MultipleLights[0]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	m_MultipleLights[0]->SetDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	m_MultipleLights[0]->SetPosition(25.0f, 10.0f, 50.0f);
	
	// Setting up a green light on the right.
	m_MultipleLights[1] = new Light();
	m_MultipleLights[1]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	m_MultipleLights[1]->SetDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	m_MultipleLights[1]->SetPosition(50.0f, 10.0f, 50.0f);

	// Setting up a blue light at the back.
	m_MultipleLights[2] = new Light();
	m_MultipleLights[2]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	m_MultipleLights[2]->SetDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	m_MultipleLights[2]->SetPosition(25.0f, 10.0f, 75.0f);

	// Setting up the different shaders.
	m_PointLightShader = new PointLightShader(m_Direct3D->GetDevice(), hwnd);
}

Lab4::~Lab4()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
	}

	if (m_PointLightShader)
	{
		delete m_PointLightShader;
		m_PointLightShader = 0;
	}

	if (m_MultipleLights)
	{
		for (int i = 0; i < 3; i++)
		{
			delete m_MultipleLights[i];
			m_MultipleLights[i] = 0;
		}
	}
}

bool Lab4::Frame()
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

bool Lab4::Render()
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

	// Translating the plane.
	worldMatrix = XMMatrixTranslation(0.0f, -10.0f, 10.0f);

	// First set of geometry data.
	// Send geometry data (from mesh)
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());

	// Set shader parameters (matrices and texture)
	m_PointLightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_MultipleLights);
	
	// Render object (combination of mesh geometry and shader process
	m_PointLightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


