// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "lab1.h"

Lab1::Lab1(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Create Mesh object
	//m_Mesh = new TriangleMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");

	// Creating a quad mesh.
	m_QuadMesh = new QuadMesh(m_Direct3D->GetDevice(), L"../res/bunny.png");

	// Loading a teapot model.
	//m_Model = new Model(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png", L"../res/teapot.obj");

	m_ColourShader = new ColourShader(m_Direct3D->GetDevice(), hwnd);
}

Lab1::~Lab1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_Mesh)
	{
		delete m_Mesh;
		m_Mesh = 0;
	}

	if (m_QuadMesh)
	{
		delete m_QuadMesh;
		m_QuadMesh = 0;
	}

	if (m_Model)
	{
		delete m_Model;
		m_Model = 0;
	}

	if (m_ColourShader)
	{
		delete m_ColourShader;
		m_ColourShader = 0;
	}
}


bool Lab1::Frame()
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

bool Lab1::Render()
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

	// Send geometry data (from mesh)
	//m_Mesh->SendData(m_Direct3D->GetDeviceContext());
	m_QuadMesh->SendData(m_Direct3D->GetDeviceContext());

	// Set shader parameters (matrices and texture)
	m_ColourShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);

	// Render object (combination of mesh geometry and shader process
	//m_ColourShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());
	m_ColourShader->Render(m_Direct3D->GetDeviceContext(), m_QuadMesh->GetIndexCount());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


