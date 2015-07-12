#ifndef OBJECT_H
#define OBJECT_H

#include "d3d/d3dApp.h"
#include "d3d/d3dShader.h"

class Object
{
public:
	Object()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		m_pLightBuffer        = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTexture            = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pDepthTexSRV,
		       const XMFLOAT4X4 &Model, const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj);

	void shutdown()
	{
			ReleaseCOM(m_pRenderTargetView  )
			ReleaseCOM(m_pMVPBuffer         )
			ReleaseCOM(m_pLightBuffer       )
			ReleaseCOM(m_pVertexBuffer      )
			ReleaseCOM(m_pIndexBuffer       )
	}

	bool load_model(char *modelFile);
	bool load_obj(char *objFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);

private:

	struct MatrixBuffer
	{
		XMFLOAT4X4  model;
		XMFLOAT4X4  view;
		XMFLOAT4X4  proj;

	};
	MatrixBuffer cbMatrix;

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
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

	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11Buffer             *m_pMVPBuffer;
	ID3D11Buffer             *m_pLightBuffer;
	ID3D11Buffer             *m_CameraBuffer;
	ID3D11Buffer             *m_pVertexBuffer;
	ID3D11Buffer             *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTexture;
	ID3D11SamplerState       *m_pTexSamplerClamp;
	ID3D11SamplerState       *m_pTexSamplerWrap;
	ID3D11InputLayout        *m_pInputLayout;


	int m_VertexCount;
	int m_IndexCount;
	std::vector<Vertex> VertexData;
	std::vector<unsigned int> IndexData;

	Shader ObjectShader;
};

#endif