// Lab8.cpp
#include "Lab8.h"

Lab8::Lab8(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Setting up local variables.
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
	m_ManipulationFrequency = 0.3f;
	m_ManipulationHeight = 1.0f;
	m_ManipulationWidth = 1.0f;
	m_Input = in;

	// Setting up the camera to be used with the ortho mesh.
	m_OrthoCamera = new Camera();
	m_OrthoCamera->SetPosition(-1.0f, -8.0f, 2.0f);
	m_OrthoCamera->SetRotation(10.0f, 5.0f, 0.0f);

	// Creating new mesh.
	//m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), 200, 150, -300, 225);
	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0.0f, 0.0f);
	m_HalfOrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), (screenWidth * 0.5f), (screenHeight * 0.5f), 0, 0);
	m_CubeMesh = new CubeMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.jpg");

	// Setting up the lighting.
	m_Light = new Light();
	m_Light->SetAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 0.0f);

	// Setting up the different shaders.
	m_LightShader = new LightShader(m_Direct3D->GetDevice(), hwnd);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_DownSampleTexture = new RenderTexture(m_Direct3D->GetDevice(), (screenWidth * 0.5f), (screenHeight * 0.5f), SCREEN_NEAR, SCREEN_DEPTH);
	m_BoxBlurShader = new BoxBlurShader(m_Direct3D->GetDevice(), hwnd);
}

Lab8::~Lab8()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_OrthoCamera)
	{
		delete m_OrthoCamera;
		m_OrthoCamera = 0;
	}

	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_BoxBlurShader)
	{
		delete m_BoxBlurShader;
		m_BoxBlurShader = 0;
	}

	if (m_OrthoMesh)
	{
		delete m_OrthoMesh;
		m_OrthoMesh = 0;
	}

	if (m_CubeMesh)
	{
		delete m_CubeMesh;
		m_CubeMesh = 0;
	}

	if (m_RenderTexture)
	{
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_DownSampleTexture)
	{
		delete m_DownSampleTexture;
		m_DownSampleTexture = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_HorizontalBlurShader)
	{
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	if (m_VerticalBlurShader)
	{
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}
}

void Lab8::Controls(float dt)
{
	// Changing frequency.
	if (m_Input->isKeyDown('O'))
	{
		m_ManipulationFrequency += 0.01f;
	}
	else if (m_Input->isKeyDown('U') && m_ManipulationFrequency > 0.0f)
	{
		m_ManipulationFrequency -= 0.01f;
	}

	// Changing width.
	if (m_Input->isKeyDown('L'))
	{
		m_ManipulationWidth += 0.01f;
	}
	else if (m_Input->isKeyDown('J') && m_ManipulationWidth > 1.0f)
	{
		m_ManipulationWidth -= 0.01f;
	}

	// Changing height.
	if (m_Input->isKeyDown('I'))
	{
		m_ManipulationHeight += 0.01f;
	}
	else if (m_Input->isKeyDown('K') && m_ManipulationHeight > 1.0f)
	{
		m_ManipulationHeight -= 0.01f;
	}
}

bool Lab8::Frame()
{
	bool result;

	//m_Rotation += 0.01f;

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

void Lab8::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to the texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//ortho_camera_->Update();
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	//ortho_camera_->GetViewMatrix(viewMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Geometry data.
	m_CubeMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shader data.
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_CubeMesh->GetTexture(), m_Light);
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_CubeMesh->GetIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();
}

void Lab8::DownSample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	m_DownSampleTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to the texture.
	m_DownSampleTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	//ortho_camera_->GetViewMatrix(viewMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Turning off the z buffer.
	m_Direct3D->TurnZBufferOff();

	// Simple texture render using the ortho matrix.
	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix, m_OrthoMesh->GetTexture());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

	// Turning the z buffer back on.
	m_Direct3D->TurnZBufferOn();
}

void Lab8::HorizontalBlur()
{
	
}

void Lab8::VerticalBlur()
{

}

void Lab8::UpSample()
{

}

void Lab8::RenderScene()
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

	// Geometry data.
	// Send geometry data (from mesh)
	m_CubeMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shader data.
	// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_CubeMesh->GetTexture(), m_Light);

	// Render object (combination of mesh geometry and shader process.
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_CubeMesh->GetIndexCount());

	// To render ortho mesh.
	// Turn off the z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	m_Direct3D->GetOrthoMatrix(orthoMatrix);	// Ortho Matrix for 2D rendering.
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// Render the ortho mesh.
	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shader data.
	//m_BoxBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView(), m_ScreenWidth, m_ScreenHeight);
	//m_BoxBlurShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

	// Turn on the z buffer to render back to the 3D scene.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
}

bool Lab8::Render()
{
	// Render the scene to a texture.
	RenderToTexture();

	// Down sample prior to blurring, optimise.
	DownSample();

	// Applying the horizontal blur stage.
	HorizontalBlur();

	// Applying the vertical blue stage.
	VerticalBlur();

	// Up sample, return to the screen size before outputting to the screen.
	UpSample();

	// Render the scene to the buffer.
	RenderScene();

	return true;
}


