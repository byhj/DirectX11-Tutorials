#ifndef Plane_H
#define Plane_H

#include <windows.h>
#include <xnamath.h>
#include <vector>

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"
#include "d3d/d3dLight.h"

namespace byhj
{


class Plane
{
public:
	Plane();
	~Plane();

	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const MatrixBuffer &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);
	void load_model(char *modelFile);

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	LightBuffer cbLight;

	struct CameraBuffer
	{
		XMFLOAT3 camPos;
		float padding;
	};

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};

	std::vector<D3DPointLight> pointLights;
	MatrixBuffer cbMatrix;
	ID3D11InputLayout        *m_pInputLayout      = nullptr;
	ID3D11VertexShader       *m_pVS               = nullptr;
	ID3D11PixelShader        *m_pPS               = nullptr;
	ID3D11Buffer             *m_pMVPBuffer        = nullptr;
	ID3D11Buffer             *m_pVertexBuffer     = nullptr;
	ID3D11Buffer             *m_pIndexBuffer      = nullptr;
	ID3D11Buffer             *m_pLightBuffer      = nullptr;
	ID3D11ShaderResourceView *m_pTexture          = nullptr;
	ID3D11SamplerState       *m_pTexSamplerState  = nullptr;
	byhj::Shader PlaneShader;

	std::vector<Vertex> m_VertexData;
	std::vector<DWORD> m_IndexData;
};


}


#endif 