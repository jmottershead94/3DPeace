// Application.h
#ifndef _LAB8_H
#define _LAB8_H

// Includes
#include "baseapplication.h"
#include "Timer.h"
#include "Input.h"
#include "OrthoMesh.h"
#include "CubeMesh.h"
#include "PlaneMesh.h"
#include "RenderTexture.h"
#include "LightShader.h"
#include "TextureShader.h"
#include "ManipulationShader.h"
#include "BoxBlurShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"

class Lab8 : public BaseApplication
{

	public:
		Lab8(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
		~Lab8();
		bool Frame();

	private:
		void Controls(float dt);
		void RenderToTexture();
		void DownSample();
		void HorizontalBlur();
		void VerticalBlur();
		void UpSample();
		void RenderScene();
		bool Render();

	private:
		// Attributes.
		float m_ScreenWidth, m_ScreenHeight;
		float m_ManipulationFrequency, m_ManipulationHeight, m_ManipulationWidth;
		float m_Rotation;
		Camera* m_OrthoCamera;
		Input* m_Input;
		TextureShader* m_TextureShader;
		Light* m_Light;
		LightShader* m_LightShader;
		OrthoMesh* m_OrthoMesh;
		OrthoMesh* m_HalfOrthoMesh;
		CubeMesh* m_CubeMesh;
		BoxBlurShader* m_BoxBlurShader;
		RenderTexture* m_RenderTexture;
		RenderTexture* m_DownSampleTexture;
		HorizontalBlurShader* m_HorizontalBlurShader;
		VerticalBlurShader* m_VerticalBlurShader;


};

#endif