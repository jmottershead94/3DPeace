// Lab10.cpp
#include "Lab10.h"

Lab10::Lab10(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Setting up local variables.
	m_Input = in;

	// Lights.
	m_Light = new Light();
	m_Light->SetAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetPosition(12.0f, 0.0f, 5.0f);
	m_Light->SetLookAt(0.0f, 0.0f, 0.0f);				// Pointing the light to look at the origin.

	// Generating a view for the light.
	// Just generate here if the light is static.
	m_Light->GenerateViewMatrix();
	m_Light->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
	
	// Meshes.
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/brick1.dds");
	
	// Shaders.
	m_PointLightShader = new PointLightShader(m_Direct3D->GetDevice(), hwnd);
	m_DepthShader = new DepthShader(m_Direct3D->GetDevice(), hwnd);
	m_ShadowShader = new ShadowShader(m_Direct3D->GetDevice(), hwnd);
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

	// Models.
	m_Teapot = new Model(m_Direct3D->GetDevice(), L"../res/brick1.dds", L"../res/teapot.obj");
}

Lab10::~Lab10()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects.
	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = nullptr;
	}

	if (m_Teapot)
	{
		delete m_Teapot;
		m_Teapot = nullptr;
	}

	if (m_PointLightShader)
	{
		delete m_PointLightShader;
		m_PointLightShader = nullptr;
	}

	if (m_DepthShader)
	{
		delete m_DepthShader;
		m_DepthShader = nullptr;
	}

	if (m_ShadowShader)
	{
		delete m_ShadowShader;
		m_ShadowShader = nullptr;
	}

	if (m_RenderTexture)
	{
		delete m_RenderTexture;
		m_RenderTexture = nullptr;
	}
}

void Lab10::Controls(float dt)
{
	// Moving the light up.
	if (m_Input->isKeyDown(VK_NUMPAD2))
	{
		m_Light->SetPosition(m_Light->GetPosition().x, m_Light->GetPosition().y + 1.0f, m_Light->GetPosition().z);
	}
	// Moving the light down.
	else if (m_Input->isKeyDown(VK_NUMPAD8))
	{
		m_Light->SetPosition(m_Light->GetPosition().x, m_Light->GetPosition().y - 1.0f, m_Light->GetPosition().z);
	}

	// Moving the light left.
	if (m_Input->isKeyDown(VK_NUMPAD4))
	{
		m_Light->SetPosition(m_Light->GetPosition().x - 1.0f, m_Light->GetPosition().y, m_Light->GetPosition().z);
	}
	// Moving the light right.
	else if (m_Input->isKeyDown(VK_NUMPAD6))
	{
		m_Light->SetPosition(m_Light->GetPosition().x + 1.0f, m_Light->GetPosition().y, m_Light->GetPosition().z);
	}
}

bool Lab10::Frame()
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

void Lab10::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, lightViewMatrix, lightProjectionMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to the texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Generating a view for the light.
	m_Light->GenerateViewMatrix();
	m_Light->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
	
	// Get the world, view, and projection from the light and direct 3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	lightViewMatrix = m_Light->GetViewMatrix();
	lightProjectionMatrix = m_Light->GetProjectionMatrix();

	// Translating the plane into the view of the camera.
	worldMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// Geometry data for the teapot.
	m_Teapot->SendData(m_Direct3D->GetDeviceContext());

	// Shader data for the teapot.
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_Teapot->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Translating the plane into the view of the camera.
	worldMatrix = XMMatrixTranslation(-50.0f, -4.0f, -25.0f);

	// Geometry data for the plane.
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shader data for the plane.
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Resetting the viewport.
	m_Direct3D->ResetViewport();
}

bool Lab10::Render()
{
	RenderToTexture();

	XMMATRIX worldMatrix, viewMatrix, lightViewMatrix, lightProjectionMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Generate this here if the light is not static.
	// Generating a view for the light matrices.
	m_Light->GenerateViewMatrix();
	//m_Light->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	lightViewMatrix = m_Light->GetViewMatrix();
	lightProjectionMatrix = m_Light->GetProjectionMatrix();

	// Handle user input.
	Controls(m_Timer->GetTime());
	
	// Scaling the teapot down into view.
	worldMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// Geometry data for the teapot.
	m_Teapot->SendData(m_Direct3D->GetDeviceContext());

	// Shader data for the teapot.
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Teapot->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_Teapot->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Translating the plane into the view of the camera.
	worldMatrix = XMMatrixTranslation(-50.0f, -4.0f, -25.0f);

	// Geometry data for the plane.
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());

	// Shader data for the plane.
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_Light);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
