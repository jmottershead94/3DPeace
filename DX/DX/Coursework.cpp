// Coursework.cpp
#include "Coursework.h"

Coursework::Coursework(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Creating a sphere mesh.
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/White.png");
	pointlights = new PointLightMesh(m_Direct3D->GetDevice(), L"../res/White.png", XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

	/*for (int i = 0; i < 3; i++)
	{
		pointlights[i] = new PointLightMesh(m_Direct3D->GetDevice(), L"../res/White.png", XMFLOAT3(i, i, i), XMFLOAT3(25.0f * i, 25.0f, 25.0f* i));
	}*/

	//// Setting up the lighting.
	//// Setting up a red light on the left.
	//m_MultipleLights[0] = new Light();
	//m_MultipleLights[0]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	//m_MultipleLights[0]->SetDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	//m_MultipleLights[0]->SetPosition(25.0f, 10.0f, 50.0f);

	//// Setting up a green light on the right.
	//m_MultipleLights[1] = new Light();
	//m_MultipleLights[1]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	//m_MultipleLights[1]->SetDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	//m_MultipleLights[1]->SetPosition(50.0f, 10.0f, 50.0f);

	//// Setting up a blue light at the back.
	//m_MultipleLights[2] = new Light();
	//m_MultipleLights[2]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	//m_MultipleLights[2]->SetDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	//m_MultipleLights[2]->SetPosition(25.0f, 10.0f, 75.0f);

	// Setting up the different shaders.
	pointlightShader = new PointLightShader(m_Direct3D->GetDevice(), hwnd);
	m_PointLightShader = new PointLightMeshShader(m_Direct3D->GetDevice(), hwnd);
}

Coursework::~Coursework()
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

	/*if (pointlights)
	{
		for (int i = 0; i < 3; i++)
		{
			delete pointlights[i];
			pointlights[i] = nullptr;
		}
	}*/

	/*if (m_MultipleLights)
	{
		for (int i = 0; i < 3; i++)
		{
			delete m_MultipleLights[i];
			m_MultipleLights[i] = nullptr;
		}
	}*/
}

bool Coursework::Frame()
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

void Coursework::RenderTheScene()
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

	// Set shaders.
	m_PointLightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), pointlights);

	// Render object (combination of mesh geometry and shader process
	m_PointLightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	
	// Render all of the point lights with their spheres in the correct position.
	for (int i = 0; i < 3; i++)
	{
		// Translating the point light spheres.
		worldMatrix = XMMatrixTranslation(pointlights->GetPositions()[i].x, pointlights->GetPositions()[i].y, pointlights->GetPositions()[i].z);

		// Render the point light sphere.
		pointlights->GetSphereMeshes()[i]->SendData(m_Direct3D->GetDeviceContext());

		// Render object (combination of mesh geometry and shader process
		m_PointLightShader->Render(m_Direct3D->GetDeviceContext(), pointlights->GetSphereMeshes()[i]->GetIndexCount());

		// Reset the world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);
	}	

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
}

bool Coursework::Render()
{
	RenderTheScene();

	return true;
}


