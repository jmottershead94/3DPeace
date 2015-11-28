// Header guard.
#ifndef _QUADMESH_H_
#define _QUADMESH_H_

// Include files.
#include "BaseMesh.h"

// Quad mesh IS A base mesh, therefore inherits from it.
class QuadMesh : public BaseMesh
{

	public:
		// Methods.
		QuadMesh(ID3D11Device* device, WCHAR* textureFilename);
		~QuadMesh();

	protected:
		// Methods.
		void InitBuffers(ID3D11Device* device);

};

#endif

