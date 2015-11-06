#ifndef Fade_H
#define Fade_H

#include "d3d/App.h"
#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{


class Fade
{
public:
	Fade()
	{
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pTexture,const XMFLOAT4X4 &Model,  
		        const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj, float fadeAmount);

	void Shutdown();

	void init_window(float posX, float posY, float width, float height);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);

private:

	d3d::MatrixBuffer cbMatrix;

	struct  Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	struct  FadeBuffer
	{
		float fadeAmount;
		float padding[3];
	};
	FadeBuffer cbFade;

	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	ComPtr<ID3D11InputLayout>  m_pInputLayout;
	ComPtr<ID3D11Buffer>       m_pFadeBuffer;

	int m_VertexCount;
	int m_IndexCount;

    float m_posX  ;
	float m_posY  ; 
	float m_width ; 
	float m_height;

	d3d::Shader FadeShader;
};

}

#endif