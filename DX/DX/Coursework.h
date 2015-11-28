// Application.h
#ifndef _COURSEWORK_H
#define _COURSEWORK_H

// Includes
#include "baseapplication.h"
#include "PlaneMesh.h"
#include "PointLightShader.h"
#include "PointLightMeshShader.h"
#include "PointLightMesh.h"

class Coursework : public BaseApplication
{
public:

	Coursework(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Coursework();
	bool Frame();

private:
	bool Render();
	void RenderTheScene();

private:
	// Attributes.
	PointLightShader* pointlightShader;
	PointLightMeshShader* m_PointLightShader;
	Light* m_MultipleLights[3];
	PointLightMesh* pointlights;
	PlaneMesh* m_PlaneMesh;

};

#endif