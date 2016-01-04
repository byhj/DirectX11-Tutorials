#ifndef RTT_H
#define RTT_H

#include "d3d/Shader.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "d3d/Utility.h"
#include <d3d11.h>


namespace byhj
{

namespace d3d
{

class RTT
{
public:
	RTT() = default;

	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pTexture, 
		        const XMFLOAT4X4 &Model,const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj);

	void Update();
	void Shutdown();
	void SetPos(float sw, float sh, float posX, float posY, float width, float height);


private:
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	d3d::MatrixBuffer cbMatrix;

	struct  Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Buffer          > m_pMVPBuffer;
	ComPtr<ID3D11Buffer          > m_pVertexBuffer;
	ComPtr<ID3D11Buffer          > m_pIndexBuffer;
	ComPtr<ID3D11SamplerState    > m_pTexSamplerState;
	ComPtr<ID3D11InputLayout     > m_pInputLayout;


	int m_VertexCount;
	int m_IndexCount;

	float m_sw;
	float m_sh;
    float m_posX  ;
	float m_posY  ; 
	float m_width ; 
	float m_height;

	Shader RTTShader;
};

}

}

#endif