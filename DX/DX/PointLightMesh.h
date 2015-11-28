#ifndef _POINTLIGHTMESH_H_
#define _POINTLIGHTMESH_H_

#include <array>
#include "Light.h"
#include "SphereMesh.h"

class PointLightMesh
{

	public:
		// Methods.
		PointLightMesh(ID3D11Device* device, WCHAR* textureFilename, XMFLOAT3 diffuseColour, XMFLOAT3 renderPosition);
		~PointLightMesh();

		// Getters.
		// This will return the spheres for each point light.
		inline std::array<SphereMesh*, 3> GetSphereMeshes() { return pointlightSpheres; }
		
		// This will return the positions for each point light.
		inline std::array<XMFLOAT3, 3> GetPositions() { return positions; }

		// This function will return the array of lights for the point light.
		inline std::array<Light*, 3> GetPointLights() { return lights; };

	private:
		// Attributes.
		std::array<SphereMesh*, 3> pointlightSpheres;
		std::array<XMFLOAT3, 3> positions;
		std::array<Light*, 3> lights;
};

#endif