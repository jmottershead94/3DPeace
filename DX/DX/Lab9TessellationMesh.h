// Tessellation Mesh, point list for tessellation
#ifndef _LAB9TESSELLATIONMESH_H_
#define _LAB9TESSELLATIONMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class Lab9TessellationMesh : public BaseMesh
{

public:
	Lab9TessellationMesh(ID3D11Device* device, WCHAR* textureFilename);
	~Lab9TessellationMesh();

	void SendData(ID3D11DeviceContext*);

protected:
	void InitBuffers(ID3D11Device* device);

};

#endif