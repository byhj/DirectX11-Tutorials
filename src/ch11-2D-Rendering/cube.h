#include <D3DX11.h>
#include <xnamath.h>

#include "d3d/d3dShader.h"
#include "d3d/d3dDebug.h"

#include <vector>

class Cube
{
public:
	Cube()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		m_pLightBuffer        = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTexture            = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext);

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
	bool init_camera(float aspect);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);

private:


private:
	struct CameraBuffer
	{
		XMFLOAT3 camPos;
		float padding;
	};

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
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
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
	ID3D11SamplerState       *m_pTexSamplerState;
	ID3D11InputLayout        *m_pInputLayout;


	int m_VertexCount;
	int m_IndexCount;
	std::vector<Vertex> VertexData;
	std::vector<unsigned int> IndexData;

	Shader TestShader;

	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Proj;

	XMVECTOR camPos;
	XMVECTOR camTarget;
	XMVECTOR camUp;
};
