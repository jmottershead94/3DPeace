// Application.h
#ifndef _LAB9_H
#define _LAB9_H

// Includes
#include "baseapplication.h"
#include "Input.h"
#include "TessellationMesh.h"
#include "TessellationShader.h"
#include "Lab9TessellationMesh.h"
#include "QuadTessellationShader.h"
#include "Timer.h"

class Lab9 : public BaseApplication
{

public:
	Lab9(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab9();
	bool Frame();

private:
	bool Render();
	void Controls(float dt);
	void RenderToTexture();

private:
	// Attributes.
	float m_TessellationFactor;
	Input* m_Input;
	TessellationShader* m_TessellationShader;
	TessellationMesh* m_TessellationMesh;
	Lab9TessellationMesh* m_NewTessellationMesh;
	QuadTessellationShader* m_QuadTessellationShader;

};

#endif