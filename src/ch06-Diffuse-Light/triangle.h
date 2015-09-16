#ifndef Triangle_H
#define Triangle_H

#include <windows.h>
#include <xnamath.h>
#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{


class Triangle
{
public:
	Triangle();
	~Triangle();

	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void Update();
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);


	struct LightBuffer
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
	LightBuffer cbLight;

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};

	d3d::MatrixBuffer cbMatrix;
	ID3D11InputLayout        *m_pInputLayout      = nullptr;
	ID3D11Buffer             *m_pMVPBuffer        = nullptr;
	ID3D11Buffer             *m_pVertexBuffer     = nullptr;
	ID3D11Buffer             *m_pIndexBuffer      = nullptr;
	ID3D11Buffer             *m_pLightBuffer      = nullptr;
	ID3D11ShaderResourceView *m_pTexture          = nullptr;
	ID3D11SamplerState       *m_pTexSamplerState  = nullptr;

	int m_VertexCount;
	int m_IndexCount;

	d3d::Shader TriangleShader;
};


}


#endif 