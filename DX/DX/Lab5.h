// Application.h
#ifndef _LAB5_H
#define _LAB5_H

// Includes
#include "baseapplication.h"

#include "Input.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "ColourShader.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "PointLightShader.h"
#include "ManipulationShader.h"
#include "Timer.h"

class Lab5 : public BaseApplication
{
public:

	Lab5(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab5();

	bool Frame();

private:
	bool Render();
	void HandleInput(Timer* dt);

private:
	// Attributes.
	float rotation, manipulation_frequency_, manipulation_height_, manipulation_width_;
	Input* input_;
	ColourShader* m_ColourShader;
	TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	PointLightShader* m_PointLightShader;
	ManipulationShader* m_ManipulationShader;
	Light* m_Light;
	Light* m_MultipleLights[3];
	SphereMesh* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	CubeMesh* m_CubeMesh;

};

#endif