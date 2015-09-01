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
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const MatrixBuffer &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};


	MatrixBuffer cbMatrix;
	ID3D11InputLayout       *m_pInputLayout  = nullptr;
	ID3D11Buffer            *m_pMVPBuffer    = nullptr;
	ID3D11Buffer            *m_pVertexBuffer = nullptr;
	ID3D11Buffer            *m_pIndexBuffer  = nullptr;

	int m_VertexCount = 0;
	int m_IndexCount  = 0;

	d3d::Shader TriangleShader;
};


}


#endif 