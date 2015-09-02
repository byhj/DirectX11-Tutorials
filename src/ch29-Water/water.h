#ifndef WATER_H
#define WATER_H


#include "d3d/App.h"
#include "d3d/Shader.h"

namespace byhj
{


class Water
{
public:
	Water()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,
		const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj, const XMFLOAT4X4 &relfectMat);

	void shutdown();

	bool LoadModel(char *modelFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile, ID3D11ShaderResourceView *m_pTexture);

private:


	struct MatrixBuffer
	{
		XMFLOAT4X4  model;
		XMFLOAT4X4  view;
		XMFLOAT4X4  proj;
		XMFLOAT4X4  reflectMat;

	};
	MatrixBuffer cbMatrix;
	struct WaterBufer
	{
		FLOAT waterTranslation;
		FLOAT reflectRefractScale;
		XMFLOAT2 padding;
	};
	WaterBufer water;

	struct ModelVertex
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	ModelVertex  *pModelVertex;

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Normal;
	};

	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11Buffer             *m_pMVPBuffer;
	ID3D11Buffer             *m_pWaterBuffer;
	ID3D11Buffer             *m_CameraBuffer;
	ID3D11Buffer             *m_pVertexBuffer;
	ID3D11Buffer             *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTexture;
	ID3D11SamplerState       *m_pTexSamplerState;
	ID3D11InputLayout        *m_pInputLayout;

	int m_VertexCount;
	int m_IndexCount;
	d3d::Shader WaterShader;
};



}

#endif