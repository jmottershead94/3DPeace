// Application.h
#ifndef _LAB10_H
#define _LAB10_H

// Includes
#include "baseapplication.h"
#include "Input.h"
#include "PlaneMesh.h"
#include "Model.h"
#include "Timer.h"
#include "PointLightShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"

class Lab10 : public BaseApplication
{

public:
	Lab10(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab10();
	bool Frame();

private:
	bool Render();
	void Controls(float dt);
	void RenderToTexture();

private:
	// Attributes.
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;
	const float SCREEN_NEAR = 0.1f;
	const float SCREEN_DEPTH = 100.0f;
	Input* m_Input;
	PlaneMesh* m_PlaneMesh;
	Model* m_Teapot;
	Light* m_Light;
	PointLightShader* m_PointLightShader;
	DepthShader* m_DepthShader;
	ShadowShader* m_ShadowShader;
	RenderTexture* m_RenderTexture;
};

#endif