#ifndef Triangle_H
#define Triangle_H

#include <windows.h>
#include <xnamath.h>
#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{


class Triangle
{
public:
	Triangle();
	~Triangle();

	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const MatrixBuffer &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
	};


	MatrixBuffer cbMatrix;
	ID3D11InputLayout        *m_pInputLayout      = nullptr;
	ID3D11VertexShader       *m_pVS               = nullptr;
	ID3D11PixelShader        *m_pPS               = nullptr;
	ID3D11Buffer             *m_pMVPBuffer        = nullptr;
	ID3D11Buffer             *m_pVertexBuffer     = nullptr;
	ID3D11Buffer             *m_pIndexBuffer      = nullptr;
	ID3D11ShaderResourceView *m_pTexture          = nullptr;
	ID3D11SamplerState       *m_pTexSamplerState  = nullptr;

	int m_VertexCount;
	int m_IndexCount;

	byhj::Shader TriangleShader;
};


}


#endif 