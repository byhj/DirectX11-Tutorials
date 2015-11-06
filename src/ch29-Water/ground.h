#ifndef GROUND_H
#define GROUND_H

#include "d3d/App.h"
#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{

class Ground
{
public:
	Ground()
	{
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,  
		const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj);

	void Shutdown()
	{
	}

	bool load_model(char *modelFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);

private:
	struct CameraBuffer
	{
		XMFLOAT3 camPos;
		float padding;
	};

	d3d::MatrixBuffer cbMatrix;

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		FLOAT  padding;
	};
	LightBuffer cbLight;

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};

	struct ModelVertex
	{
		float x, y , z;
		float u, v;
		float nx, ny, nz;
	};
	ModelVertex  *m_pModelVertex;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pLightBuffer;
	ComPtr<ID3D11Buffer> m_CameraBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	ComPtr<ID3D11InputLayout> m_pInputLayout;


	int m_VertexCount;
	int m_IndexCount;

	d3d::Shader GroundShader;
};


}
#endif