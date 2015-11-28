#include "PointLightMesh.h"

PointLightMesh::PointLightMesh(ID3D11Device* device, WCHAR* textureFilename, XMFLOAT3 diffuseColour, XMFLOAT3 renderPosition)
{
	
	for (int i = 0; i < 3; i++)
	{
		pointlightSpheres[i] = new SphereMesh(device, textureFilename, 20);
	}

	// Setting up the lighting.
	/*SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	SetDiffuseColour(diffuseColour.x, diffuseColour.y, diffuseColour.z, 1.0f);
	SetPosition(renderPosition.x, renderPosition.y, renderPosition.z);*/

	/*pointlight = new Light();
	pointlight->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	pointlight->SetDiffuseColour(diffuseColour.x, diffuseColour.y, diffuseColour.z, 1.0f);
	pointlight->SetPosition(position.x, position.y, position.z);*/
	
	// Setting up the lighting.
	// Setting up a red light on the left.
	lights[0] = new Light();
	lights[0]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[0]->SetDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0]->SetPosition(25.0f, 10.0f, 50.0f);
	positions[0] = { 25.0f, 10.0f, 50.0f };
	
	//Setting up a green light on the right.
	lights[1] = new Light();
	lights[1]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[1]->SetDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	lights[1]->SetPosition(50.0f, 10.0f, 50.0f);
	positions[1] = { 50.0f, 10.0f, 50.0f };
	
	// Setting up a blue light at the back.
	lights[2] = new Light();
	lights[2]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[2]->SetDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	lights[2]->SetPosition(25.0f, 10.0f, 75.0f);
	positions[2] = { 25.0f, 10.0f, 75.0f };

}

PointLightMesh::~PointLightMesh()
{
	
	

}