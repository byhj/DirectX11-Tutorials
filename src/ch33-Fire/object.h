#ifndef OBJECT_H
#define OBJECT_H

#include "d3d/App.h"
#include "d3d/Shader.h"
#include "d3d/d3dLight.h"

namespace byhj
{


class Object
{
public:
	Object()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,  
		                             const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj);

	void shutdown()
	{
			ReleaseCOM(m_pRenderTargetView  )
			ReleaseCOM(m_pMVPBuffer         )
			ReleaseCOM(m_pVertexBuffer      )
			ReleaseCOM(m_pIndexBuffer       )
	}

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

	};
	MatrixBuffer cbMatrix;

	struct DistortionBuffer
	{    
		XMFLOAT2 distortion1;
		XMFLOAT2 distortion2;
		XMFLOAT2 distortion3;
		float  distortionScale;
		float  distortionBias;
	};
	DistortionBuffer cbDistortion;

	struct NoiseBuffer
	{
		float   frameTime;
		XMFLOAT3 scrollSpeeds;
		XMFLOAT3 scales;
		float    padding;
	};
	NoiseBuffer cbNoise;

	struct Vertex
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
	ID3D11Buffer             *m_pNoiseBuffer;
	ID3D11Buffer             *m_pDistortBuffer;
	ID3D11Buffer             *m_pVertexBuffer;
	ID3D11Buffer             *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTextures[3];
	ID3D11SamplerState       *m_pTexSamplerState;
	ID3D11SamplerState       *m_pTexSamplerState1;
	ID3D11InputLayout        *m_pInputLayout;

	int m_VertexCount;
	int m_IndexCount;

	Shader ObjectShader;
	std::vector<D3DPointLight> pointLights;
};

}
#endif