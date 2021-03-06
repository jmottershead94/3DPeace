// Tessellation Mesh, point list for tessellation
#ifndef _POINTMESH_H_
#define _POINTMESH_H_

#include <vector>
#include "BaseMesh.h"

using namespace DirectX;

class PointMesh : public BaseMesh
{

public:
	PointMesh(ID3D11Device* device, WCHAR* textureFilename);
	~PointMesh();

	void SendData(ID3D11DeviceContext*);
	void InitBuffers(ID3D11Device* device);

//protected:
	//void InitBuffers(ID3D11Device* device);

};

#endif