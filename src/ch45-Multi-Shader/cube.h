#ifndef Cube_H
#define Cube_H

#include <windows.h>

#include <vector>

#include "d3d/Shader.h"



namespace byhj
{


class Cube
{
public:
	Cube();
	~Cube();

	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void Render(ID3D11DeviceContext *pD3D11DeviceContext);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void load_model(char *modelFile);


	ID3D11Buffer  *m_pVertexBuffer     = nullptr;
	ID3D11Buffer  *m_pIndexBuffer      = nullptr;

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};
	std::vector<Vertex> m_VertexData;
	std::vector<DWORD> m_IndexData;
};


}


#endif 