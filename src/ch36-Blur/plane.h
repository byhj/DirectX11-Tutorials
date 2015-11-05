#ifndef PLANE_H
#define PLANE_H

#include "d3d/Shader.h"
#include "DirectXTK/DDSTextureLoader.h"
#include <d3d11.h>


namespace byhj
{

class Plane
{
public:
	Plane(){}
	~Plane(){}
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pTexture);

	void Shutdown()
	{
		ReleaseCOM(m_pVertexBuffer      )
		ReleaseCOM(m_pIndexBuffer       )
	}

	void init_window(float posX, float posY, float width, float height, float aspect);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);

private:



	struct  Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	ID3D11Buffer             *m_pVertexBuffer    = nullptr;
	ID3D11Buffer             *m_pIndexBuffer     = nullptr;
	ID3D11SamplerState       *m_pTexSamplerState = nullptr;

	int m_VertexCount;
	int m_IndexCount;

    float m_posX  ;
	float m_posY  ; 
	float m_width ; 
	float m_height;
	float m_aspect;
};


}

#endif