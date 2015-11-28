// Application.h
#ifndef _LAB4_H
#define _LAB4_H

// Includes
#include "baseapplication.h"
#include "PlaneMesh.h"
#include "PointLightShader.h"

class Lab4 : public BaseApplication
{
public:

	Lab4(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab4();
	bool Frame();

private:
	bool Render();

private:
	// Attributes.
	PointLightShader* m_PointLightShader;
	Light* m_MultipleLights[3];
	PlaneMesh* m_PlaneMesh;

};

#endif