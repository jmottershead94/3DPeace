// Application.h
#ifndef _LAB11_H
#define _LAB11_H

// Includes
#include <vector>
#include "baseapplication.h"
#include "Input.h"
#include "PointMesh.h"
#include "Timer.h"
#include "GeometryShader.h"

class Lab11 : public BaseApplication
{

public:
	Lab11(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Lab11();
	bool Frame();

private:
	bool Render();
	void Controls(float dt);
	inline PointMesh* GetPointMesh() { return new PointMesh(m_Direct3D->GetDevice(), NULL); }

private:
	// Attributes.
	Input* m_Input;
	GeometryShader* m_GeometryShader;
	vector<PointMesh*> m_Points;
};

#endif