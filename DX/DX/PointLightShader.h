// Light shader.h
// Basic single light shader setup
#ifndef _POINTLIGHTSHADER_H_
#define _POINTLIGHTSHADER_H_

#include "BaseShader.h"
#include "light.h"

using namespace std;
using namespace DirectX;

class PointLightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient[3];
		XMFLOAT4 diffuse[3];
		XMFLOAT4 position[3];
		/*XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 position;
		float padding;*/
	};

public:

	PointLightShader(ID3D11Device* device, HWND hwnd);
	~PointLightShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, /*Light* light*/Light* light[3]);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
};

#endif