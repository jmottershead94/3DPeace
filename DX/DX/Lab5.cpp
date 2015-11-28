// Lab5.cpp
#include "Lab5.h"

Lab5::Lab5(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Setting up local variables.
	manipulation_frequency_ = 1.0f;
	manipulation_height_ = 1.0f;
	manipulation_width_ = 1.0f;
	rotation = 0.0f;
	input_ = in;

	// Creating a new mesh.
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
	m_SphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
	m_CubeMesh = new CubeMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");

	// Setting up the lighting.
	m_Light = new Light();
	m_Light->SetAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 0.0f);

	// Setting up the different shaders.
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_LightShader = new LightShader(m_Direct3D->GetDevice(), hwnd);
	m_PointLightShader = new PointLightShader(m_Direct3D->GetDevice(), hwnd);
	m_ManipulationShader = new ManipulationShader(m_Direct3D->GetDevice(), hwnd);
}

Lab5::~Lab5()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_CubeMesh)
	{
		delete m_CubeMesh;
		m_CubeMesh = 0;
	}

	if (m_SphereMesh)
	{
		delete m_SphereMesh;
		m_SphereMesh = 0;
	}

	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
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

	if (m_PointLightShader)
	{
		delete m_PointLightShader;
		m_PointLightShader = 0;
	}

	if (m_ManipulationShader)
	{
		delete m_ManipulationShader;
		m_ManipulationShader = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
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

void Lab5::HandleInput(Timer* dt)
{
	// Changing frequency.
	if (input_->isKeyDown('O'))
	{
		manipulation_frequency_ += 0.01f;
	}
	else if (input_->isKeyDown('U') /*&& manipulation_frequency_ > 1.0f*/)
	{
		manipulation_frequency_ -= 0.01f;
	}

	// Changing width.
	if (input_->isKeyDown('L'))
	{
		manipulation_width_ += 0.01f;
	}
	else if (input_->isKeyDown('J') && manipulation_width_ > 1.0f)
	{
		manipulation_width_ -= 0.01f;
	}

	// Changing height.
	if (input_->isKeyDown('I'))
	{
		manipulation_height_ += 0.01f;
	}
	else if (input_->isKeyDown('K') && manipulation_height_ > 1.0f)
	{
		manipulation_height_ -= 0.01f;
	}
}

bool Lab5::Frame()
{
	bool result;

	//rotation += 0.01f;
	//manipulation_width_ += 0.01f;
	//manipulation_height_ += 0.01f;

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

bool Lab5::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Handle controls and user input.
	HandleInput(m_Timer);

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
	m_ManipulationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_Light, m_Timer, manipulation_frequency_, manipulation_height_, manipulation_width_);
	
	// Render object (combination of mesh geometry and shader process
	m_ManipulationShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


