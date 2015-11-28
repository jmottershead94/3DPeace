// Light shader.h
// Basic single light shader setup
#ifndef _POINTLIGHTMESHSHADER_H_
#define _POINTLIGHTMESHSHADER_H_

#include "BaseShader.h"
#include "PointLightMesh.h"

using namespace std;
using namespace DirectX;

class PointLightMeshShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient[3];
		XMFLOAT4 diffuse[3];
		XMFLOAT4 position[3];
	};

public:

	PointLightMeshShader(ID3D11Device* device, HWND hwnd);
	~PointLightMeshShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, /*Light* light*/PointLightMesh* light);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
};

#endif