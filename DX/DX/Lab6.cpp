// Lab6.cpp
#include "Lab6.h"

Lab6::Lab6(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Setting up local variables.
	manipulation_frequency_ = 1.0f;
	manipulation_height_ = 1.0f;
	manipulation_width_ = 1.0f;
	input_ = in;

	// Setting up the camera to be used with the ortho mesh.
	ortho_camera_ = new Camera();
	ortho_camera_->SetPosition(50.0f, 25.0f, 25.0f);
	ortho_camera_->SetRotation(25.0f, 0.0f, 0.0f);

	// Creating new mesh.
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/Water.jpg");
	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), 200, 150, -300, 225);
	
	// Setting up the lighting.
	m_Light = new Light();
	m_Light->SetAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 0.0f);

	// Setting up the different shaders.
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_ManipulationShader = new ManipulationShader(m_Direct3D->GetDevice(), hwnd);
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
}

Lab6::~Lab6()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_OrthoMesh)
	{
		delete m_OrthoMesh;
		m_OrthoMesh = 0;
	}
	
	if (m_RenderTexture)
	{
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
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
}

void Lab6::Controls(float dt)
{
	// Changing frequency.
	if (input_->isKeyDown('O'))
	{
		manipulation_frequency_ += 0.01f;
	}
	else if (input_->isKeyDown('U') && manipulation_frequency_ > 0.0f)
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

bool Lab6::Frame()
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

void Lab6::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to the texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);
	
	// Generate the view matrix based on the camera's position.
	ortho_camera_->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	ortho_camera_->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Translating the plane.
	worldMatrix = XMMatrixTranslation(0.0f, -10.0f, 10.0f);

	// Geometry data.
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shader data.
	m_ManipulationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_Light, m_Timer, manipulation_frequency_, manipulation_height_, manipulation_width_);
	m_ManipulationShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();
}

bool Lab6::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Rendering the scene to a 2D ortho mesh.
	RenderToTexture();

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
	
	// Translating the plane.
	worldMatrix = XMMatrixTranslation(0.0f, -10.0f, 10.0f);

	// Geometry data.
	// Send geometry data (from mesh)
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shader data.
	// Set shader parameters (matrices and texture)
	m_ManipulationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_Light, m_Timer, manipulation_frequency_, manipulation_height_, manipulation_width_);
	
	// Render object (combination of mesh geometry and shader process.
	m_ManipulationShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	
	// To render ortho mesh.
	// Turn off the z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();
	
	m_Direct3D->GetOrthoMatrix(orthoMatrix);	// Ortho Matrix for 2D rendering.
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

	// Turn on the z buffer to render back to the 3D scene.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


