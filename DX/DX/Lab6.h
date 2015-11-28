// Application.h
#ifndef _LAB6_H
#define _LAB6_H

// Includes
#include "baseapplication.h"
#include "Input.h"
#include "OrthoMesh.h"
#include "PlaneMesh.h"
#include "RenderTexture.h"
#include "TextureShader.h"
#include "ManipulationShader.h"
#include "SpecularLightShader.h"
#include "Timer.h"

class Lab6 : public BaseApplication
{

	public:
		Lab6(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
		~Lab6();
		bool Frame();

	private:
		bool Render();
		void Controls(float dt);
		void RenderToTexture();

	private:
		// Attributes.
		float manipulation_frequency_, manipulation_height_, manipulation_width_;
		Camera* ortho_camera_;
		Input* input_;
		TextureShader* m_TextureShader;
		ManipulationShader* m_ManipulationShader;
		Light* m_Light;
		PlaneMesh* m_PlaneMesh;
		OrthoMesh* m_OrthoMesh;
		RenderTexture* m_RenderTexture;

};

#endif