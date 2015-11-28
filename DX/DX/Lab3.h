// Application.h
#ifndef _LAB3_H
#define _LAB3_H

// Includes
#include "baseapplication.h"

#include "SphereMesh.h"
#include "ColourShader.h"
#include "TextureShader.h"
#include "LightShader.h"

class Lab3 : public BaseApplication
{
public:

	Lab3(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab3();

	bool Frame();

private:
	bool Render();

private:
	// Attributes.
	float rotation;
	ColourShader* m_ColourShader;
	TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	Light* m_Light;
	SphereMesh* m_SphereMesh;

};

#endif