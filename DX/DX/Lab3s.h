// Application.h
#ifndef _LAB3S_H
#define _LAB3S_H

// Includes
#include "baseapplication.h"

#include "SphereMesh.h"
#include "ColourShader.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "SpecularLightShader.h"

class Lab3s : public BaseApplication
{
public:

	Lab3s(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab3s();

	bool Frame();

private:
	bool Render();

private:
	// Attributes.
	float rotation;
	ColourShader* m_ColourShader;
	TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	SpecularLightShader* m_SpecularLightShader;
	Light* m_Light;
	SphereMesh* m_SphereMesh;

};

#endif